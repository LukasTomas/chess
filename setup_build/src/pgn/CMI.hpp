#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace CMI {
/**
 * @brief Chess Move Interface
 * A standard chess half move interface for improving chess libraries
 * interoperability.
 * NONE OF THESE METHODS IMPLEMENTATIONS DELETE A CMI::HalfMove EXCEPT
 * CMI::HalfMove::~HalfMove. It is up to the user of the object instance to do
 * it after ensuring that no references to the CMI::HalfMove remains in the move
 * tree.
 */
class HalfMove {

  std::vector<HalfMove *> variations;
  std::string SAN, comment;
  std::uint16_t number;
  bool isBlack;
  std::uint8_t NAG;
  HalfMove *parent;
  HalfMove *mainline;

  /// @brief Set the parent of current CMI::HalfMove, only used internally
  void SetParent(HalfMove *);

public:
  HalfMove();
  HalfMove(const std::string &SAN);
  HalfMove(const std::string &SAN, const std::string &comment,
           std::uint16_t number, std::uint8_t NAG, bool isBlack);
  /// @brief Ensure that the destructor of the child class is called
  virtual ~HalfMove();
  /// @brief  Return a pointer to the next CMI::HalfMove
  virtual HalfMove *GetMainline() const;
  /// @brief Bind two moves together
  virtual void SetMainline(HalfMove *);
  /// @brief Get the previous CMI::HalfMove
  virtual HalfMove *GetParent() const;
  /// @brief Return the current move using the SAN notation e.g: "Qxc5+" or "a4"
  virtual std::string GetSAN() const;
  /// @brief Setter to replace current SAN
  virtual void SetSAN(const std::string &);
  /// @brief Return the HalfMove move number e.g 1 for the first white's and
  /// black's move
  virtual std::uint16_t GetNumber() const;
  /// @brief Setter to replace current Number
  virtual void SetNumber(std::uint16_t);
  /// @brief Return the Numeric Annotation Glyphs code
  virtual std::uint8_t GetNAG() const;
  /// @brief Setter to replace current NAG
  virtual void SetNAG(std::uint8_t);
  /// @brief Return the comment linked to the current move or empty string
  virtual std::string GetComment() const;
  /// @brief Setter to replace current comment
  virtual void SetComment(const std::string &);
  /// @brief Return true if the current HalfMove was played by black
  virtual bool IsBlack() const;
  /// @brief Setter to replace that determined the return values of
  /// HalfMove::IsBlack()
  virtual void SetIsBlack(bool);
  /// @brief All the variations of the current move
  virtual std::vector<HalfMove *> GetVariations() const;
  /// @brief Add a variation to the variations list
  virtual void AddVariation(HalfMove *);
  /**
   * @brief Remove the given CMI::HalfMove from the variations list
   *
   * @return true if found and deleted
   * @return false otherwise
   */
  virtual bool RemoveVariation(HalfMove *);
  virtual void ClearVariations();
  /**
   * @brief Remove the given CMI::HalfMove from mainline and variations list
   *
   * @return true if found and deleted
   * @return false otherwise
   */
  virtual bool RemoveChild(HalfMove *);
  /// @brief Return the CMI::HalfMove root node of the current move branch
  virtual HalfMove *GetBranchRoot();
  /// @brief Promote the current variation if any
  virtual void Promote();
  /// @brief Make the current variation the main line
  virtual void SetAsMainline();
  /// @brief Check if the tree move contains a pointer to the given move
  virtual bool Contains(HalfMove *) const;
  /// @brief Check if the current tree of move is consistent
  virtual bool IsConsistent() const;
};
} // namespace CMI