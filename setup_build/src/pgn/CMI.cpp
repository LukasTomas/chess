#include "CMI.hpp"

namespace CMI {

HalfMove::HalfMove()
    : number(1), isBlack(false), NAG(0), parent(nullptr), mainline(nullptr) {}

HalfMove::HalfMove(const std::string &SAN)
    : SAN(SAN), number(1), isBlack(false), parent(nullptr), mainline(nullptr) {}

HalfMove::HalfMove(const std::string &SAN, const std::string &comment,
                   std::uint16_t number, std::uint8_t NAG, bool isBlack)
    : SAN(SAN), comment(comment), number(number), isBlack(isBlack), NAG(NAG),
      parent(nullptr), mainline(nullptr) {}

HalfMove::~HalfMove() {
  if (mainline != nullptr)
    delete mainline;
  for (HalfMove *v : variations)
    delete v;
}

void HalfMove::SetParent(HalfMove *m) { parent = m; }

std::vector<HalfMove *> HalfMove::GetVariations() const { return variations; }

HalfMove *HalfMove::GetMainline() const { return mainline; };

HalfMove *HalfMove::GetParent() const { return parent; };

std::string HalfMove::GetSAN() const { return SAN; };

void HalfMove::SetSAN(const std::string &newSAN) { SAN = newSAN; };

std::uint16_t HalfMove::GetNumber() const { return number; };

void HalfMove::SetNumber(std::uint16_t n) { number = n; };

std::uint8_t HalfMove::GetNAG() const { return NAG; };

void HalfMove::SetNAG(std::uint8_t n) { NAG = n; };

std::string HalfMove::GetComment() const { return comment; };

void HalfMove::SetComment(const std::string &c) { comment = c; };

bool HalfMove::IsBlack() const { return isBlack; };

void HalfMove::SetIsBlack(bool b) { isBlack = b; };

void HalfMove::ClearVariations() { variations.clear(); }

void HalfMove::SetMainline(HalfMove *m) {
  mainline = m;
  if (m != nullptr) {
    if (!this->isBlack) {
      m->SetIsBlack(true);
      m->SetNumber(this->number);
    } else {
      m->SetIsBlack(false);
      m->SetNumber(this->number + 1);
    }
    m->SetParent(this);
  }
}

void HalfMove::Promote() {
  HalfMove *broot = GetBranchRoot();
  if (broot != nullptr) {
    HalfMove *parent = broot->GetParent();
    if (parent != nullptr) {
      if (parent->GetMainline() != broot) {
        HalfMove *pparent = parent->GetParent();
        // First update parent of parent:
        if (pparent != nullptr) {
          if (pparent->GetMainline() == parent)
            pparent->SetMainline(broot);
          else {
            pparent->AddVariation(broot);
            pparent->RemoveChild(parent);
          }
        }
        // Now update parent:
        parent->RemoveChild(broot);
        broot->AddVariation(parent);
      }
    }
  }
}

void HalfMove::SetAsMainline() {
  HalfMove *broot = GetBranchRoot();
  HalfMove *lastRoot;
  // Just promote until we cannot anymore
  do {
    lastRoot = broot;
    broot->Promote();
    broot = GetBranchRoot();
  } while (broot != lastRoot);
}

HalfMove *HalfMove::GetBranchRoot() {
  HalfMove *m = this;
  HalfMove *p = parent;
  while (p != nullptr) {
    if (p->GetMainline() != m) {
      return (m);
    }
    m = p;
    p = m->GetParent();
  }
  return m;
}

void HalfMove::AddVariation(HalfMove *m) {
  m->SetIsBlack(IsBlack());
  m->SetNumber(GetNumber());
  m->SetParent(this);
  variations.push_back(m);
}

bool HalfMove::RemoveVariation(HalfMove *m) {
  std::vector<HalfMove *> vars;
  bool removed = false;
  for (HalfMove *v : GetVariations()) {
    if (m != v)
      vars.push_back(v);
    else
      removed = true;
  }
  if (removed)
    variations = vars;
  return removed;
}

bool HalfMove::RemoveChild(HalfMove *m) {
  if (GetMainline() == m) {
    mainline = nullptr;
    return true;
  }
  return RemoveVariation(m);
}

bool HalfMove::Contains(HalfMove *m) const {
  if (m == nullptr)
    return false;
  else if (this == m)
    return true;
  else if (mainline != nullptr && mainline->Contains(m)) {
    return true;
  } else {
    for (HalfMove *v : variations) {
      if (v->Contains(m))
        return true;
    }
  }
  return false;
}

bool HalfMove::IsConsistent() const {
  // Check mainline
  if (mainline != nullptr) {
    if (isBlack == mainline->isBlack)
      return false;
    if (isBlack && number + 1 != mainline->number)
      return false;
    if (!isBlack && number != mainline->number)
      return false;
    if (mainline->parent != this)
      return false;
    if (!mainline->IsConsistent())
      return false;
  }
  // Check variations
  for (HalfMove *v : variations) {
    if (number != v->number || isBlack != v->isBlack || v->parent != this)
      return false;
    if (!v->IsConsistent())
      return false;
  }
  return true;
}
} // namespace CMI