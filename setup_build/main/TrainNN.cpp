#include "../src/neural/SimpleNet.h"
#include "../src/neural/ConvNet.h"
#include "../src/neural/ResNet.h"

#include "../src/dataset/ChessDataset_analyzed.h"

#include "../src/terminal/Space.h"
#include "../src/terminal/ProgressShower.h"
#include "../src/terminal/Logging.h"
#include "../src/terminal/DrawingChars.h"

#include <torch/torch.h>
#include <iostream>
#include <fstream>
#include <csignal>

using namespace std;

const string MSES_FILEPATH = "values.mse";
const int MAX_CUDA_DEVICE = 3;
const size_t BATCH_SIZE = 128;

sig_atomic_t stopFlag = 0;


float train(torch::Device device, size_t kNumberOfEpochs, bool printProgress) {
    auto dataset = ChessDataset_analyzed("/home/petrzluk/chess/chess_resources/parsed_games_2013-09.chess.constraints.analyzed")
                       .map(torch::data::transforms::Stack<>());

    auto data_loader = torch::data::make_data_loader(
        std::move(dataset),
        torch::data::DataLoaderOptions().batch_size(BATCH_SIZE).workers(7));

    ConvNet model(17, 8);

    model->to(device);
    torch::optim::Adam optimizer(model->parameters(), torch::optim::AdamOptions(1e-3));

    float best_loss = std::numeric_limits<float>::max();
    float last_loss;
    vector<float> epochs_mse;
    ofstream mseFile(MSES_FILEPATH);

    for (int64_t epoch = 1; epoch <= kNumberOfEpochs; ++epoch) {
        if (epoch != 1) {
            if (printProgress)
                // one line up
                cout << "\x1b[A";
        }

        if (printProgress)
            cout << "\r";
        cout << "Epoch " << epoch << "/" << kNumberOfEpochs;
        cout << " | Best MSE: " << (epoch == 1 ? "N/A" : to_string(best_loss));
        cout << " | Last MSE: " << (epoch == 1 ? "N/A" : to_string(last_loss)) << endl;

        ProgressShower shower;

        float mse_loss = 0;
        int batchCount = 0;

        for (torch::data::Example<> &batch : *data_loader) {
            if ( stopFlag )
                return best_loss;

            batch.data = batch.data.to(device);
            batch.target = batch.target.to(device);
            // cout << batch.data.sizes() << ", " << batch.target.sizes() << endl;

            // cout << "board " << boardTensor << endl;
            // cout << "constraint " << constraintTensor << endl;

            optimizer.zero_grad();

            torch::Tensor prediction = model->forward(batch.data);
            // cout << "result " << resultTensor << endl;
            // cout << "prediction " << prediction << endl;
            torch::Tensor loss = torch::nn::functional::binary_cross_entropy(/*prediction.view({BATCH_SIZE, 1})*/prediction, batch.target.to(torch::kFloat32));
            // cout << "loss " << loss << endl;

            loss.backward();
            optimizer.step();

            mse_loss += loss.item<float>();

            if (printProgress)
                shower.showProgress(batchCount, dataset.size().value() / BATCH_SIZE);
            batchCount++;
        }

        // clear progress bar
        if ( printProgress ) {
            if (epoch != kNumberOfEpochs)
                shower.clear();
        }

        last_loss = mse_loss / batchCount;
        epochs_mse.emplace_back(last_loss);
        mseFile << to_string(last_loss) << endl;
        mseFile.flush();

        if (last_loss < best_loss)
        {
            best_loss = last_loss;
            torch::save(model, "best_model.pt");
        }
    }

    return best_loss;
}

void showHelp(string programName) {
    cout << "Usage: " << programName << " <number of epochs> <cuda device number> <print_progress=false>" << endl
         << "\t\t <number of epochs> - number of epochs to train for" << endl
         << "\t\t <cuda device number> - cuda device number to train on (0 - " << MAX_CUDA_DEVICE << ") or -1 for cpu" << endl
         << "\t\t <print_progress> - whether to print progress bar (default: false)" << endl;
}

class Program {
public:

    static constexpr double FPS = 0.3;

    static string GetStringTime( int64_t milliseconds ) {
        int seconds = milliseconds / 1000;
        string time;

        int64_t hours = seconds / 3600;
        seconds -= hours * 3600;
        if ( hours > 0 )
            time += to_string(hours) + "h";

        int64_t minutes = seconds / 60;
        seconds -= minutes * 60;
        if ( minutes > 0 ) {
            if ( time.length() != 0 )
                time += " ";
            time += to_string(minutes) + "m";
        }

        if ( seconds > 0 ) {
            if ( time.length() != 0 )
                time += " ";
            time += to_string(seconds) + "s";
        }

        return time;
    }

    Program( char * argv[] ) {
        _programName = argv[0];
        _startTime = chrono::high_resolution_clock::now();
    }

    ~Program() {
        int64_t duration = GetDuration();
        cout << "Program finished in " << GetStringTime(duration) << endl;
    }

    void Start() {
        // while ( !stopFlag ) {
        //     Render();
        //     this_thread::sleep_for(chrono::milliseconds((int)(1000 * FPS)));
        // }
    }

private:

    string _programName;
    chrono::time_point<std::chrono::high_resolution_clock> _startTime;

    Space space;

    int64_t GetDuration() {
        auto endTime = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - _startTime).count();
        return duration;
    }

    void Render() {
        const size_t nameSize = _programName.length();
        int widthRemains = space.GetWidth() - nameSize - 2; // 2 for spaces
        int leftWidth = widthRemains / 2;
        int rightWidth = widthRemains - leftWidth;

        string duration = GetStringTime(GetDuration());
        rightWidth -= duration.length() + 1; // 1 for space

        string firstLine;
        boxDrwaing::AddMultipleChars(firstLine, boxDrwaing::LIGHT_HORIZONTAL, leftWidth);
        firstLine += " " + _programName + " ";
        boxDrwaing::AddMultipleChars(firstLine, boxDrwaing::LIGHT_HORIZONTAL, rightWidth);
        firstLine += " " + duration;

        LOG_NOTICE << firstLine << endl;
    }
};

int main(int argc, char *argv[]) {
    Program program(argv);
    program.Start();

    // signal(SIGINT, [](int signum) {
    //     LOG_ERROR << "SIGINT received, exiting..." << endl;
    //     stopFlag = 1;
    // });

    if (argc != 3 && argc != 4) {
        showHelp(argv[0]);
        return 1;
    }

    cout << "INFO:\tcuda availability: " << (torch::cuda::is_available() ? "true" : "false") << endl;
    cout << "\tcudnn availability: " << (torch::cuda::cudnn_is_available() ? "true" : "false") << endl;

    const size_t kNumberOfEpochs = stoi(argv[1]);

    const int cudaNumber = stoi(argv[2]);
    string deviceName;
    if (cudaNumber == -1)
        deviceName = "cpu";
    else if (cudaNumber >= 0 && cudaNumber <= MAX_CUDA_DEVICE) {
        if (!torch::cuda::is_available())
        {
            cout << "CUDA is not available!" << endl;
            return 2;
        }
        deviceName = "cuda:" + to_string(cudaNumber);
    }
    else {
        cout << "Invalid cuda device number" << endl;
        showHelp(argv[0]);
        return 1;
    }

    cout << "Will train on " << deviceName << " for " << kNumberOfEpochs << " epochs" << endl
         << endl;

    torch::Device device(deviceName);
    // cout << "Will use kCUDA: " << torch::kCUDA << endl;
    // torch::Device device(torch::kCUDA);

    bool printProgress = false;
    if ( argc == 4 ) {
        string boolString = argv[3];
        if (boolString != "false" && boolString != "true") {
            cerr << "Invalid print_progress argument, only possible values are {\"true\", \"false\"}" << endl;
            return 1;
        }

        printProgress = boolString == "true";
    }

    float bestLoss = train(device, kNumberOfEpochs, printProgress);
    cout << endl
         << "Best MSE: " << to_string(bestLoss) << endl;

    return 0;
}
