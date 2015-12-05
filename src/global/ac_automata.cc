#include "global/ac_automata.h"

#include <algorithm>

namespace global {

AcAutomata::Node::Node() : depth_(0), final_(false),
    pattern_(NULL), failure_node_(NULL), parent_(NULL) {
}

AcAutomata::Node::~Node() {
}

AcAutomata::Node *AcAutomata::Node::CreateNext(char alpha) {
  Node *next = new Node();
  next->depth_ = this->depth_ + 1;
  next->parent_ = this;

  Edge edge;
  edge.alpha_ = alpha;
  edge.next_ = next;
  this->children_.push_back(edge);

  return next;
}

void AcAutomata::Node::RegisterPattern(const std::string *pattern) {
  this->pattern_ = pattern;
}

void AcAutomata::Node::SortEdges() {
  std::sort(this->children_.begin(), this->children_.end(), EdgeLess());
}

AcAutomata::Node *AcAutomata::Node::FindNext(char alpha) {
  for (size_t i = 0; i < this->children_.size(); ++i) {
    if (this->children_[i].alpha_ == alpha) {
      return this->children_[i].next_;
    }
  }
  
  return NULL;
}

AcAutomata::Node *AcAutomata::Node::FindNextBS(char alpha) {
  if (this->children_.empty()) {
    return NULL;
  }

  int min = 0;
  int max = this->children_.size() - 1;

  while (min <= max) {
    int mid = (min + max) / 2;
    char mid_alpha = this->children_[mid].alpha_;
    if (alpha > mid_alpha) {
      min = mid + 1;
    } else if (alpha < mid_alpha) {
      max = mid - 1;
    } else {
      return this->children_[mid].next_;
    }
  }

  return NULL;
}

bool AcAutomata::EdgeLess::operator()(const Edge &lhs, const Edge &rhs) {
  return lhs.alpha_< rhs.alpha_;
}

///////////////////////////////////////////////////////////////////////////////

AcAutomata::AcAutomata() : compiled_(false), position_(0) {
  this->root_ = new Node();
  this->all_nodes_.push_back(this->root_);
  this->current_ = this->root_;
}

AcAutomata::~AcAutomata() {
  for (size_t i = 0; i < this->all_nodes_.size(); ++i) {
    delete this->all_nodes_[i];
  }
}

bool AcAutomata::AddPattern(const std::string &pattern) {
  if (this->compiled_) {
    return false;
  }
  if (pattern.empty()) {
    return false;
  }

  Node *current = this->root_;

  for (size_t i = 0; i < pattern.size(); ++i) {
    char alpha = pattern[i];
    Node *next = current->FindNext(alpha);
    if (next != NULL) {
      current = next;
      continue;
    } else {
      next = current->CreateNext(alpha);
      current = next;
      this->all_nodes_.push_back(next);
    }
  }

  if (current->CheckFinal()) {
    return false;
  }

  this->patterns_store_.push_back(pattern);
  current->SetFinal();
  current->RegisterPattern(&this->patterns_store_.back());

  return true;
}

void AcAutomata::TranverseSetFailureNode(Node *node) {
  for (size_t i = 0; i < node->GetChildren().size(); ++i) {
    Node *child = node->GetChildren()[i].next_;
    char alpha = node->GetChildren()[i].alpha_;
    Node *next = child;

    for (;;) {
      if (NULL == next->GetParentNode()) {
        child->SetFailureNode(this->root_);
        break;
      }

      Node *parent_failure_node = next->GetParentNode()->GetFailureNode();
      if (NULL == parent_failure_node) {
        child->SetFailureNode(this->root_);
        break;
      }

      Node *failure_node = parent_failure_node->FindNext(alpha);
      if (failure_node != NULL) {
        child->SetFailureNode(failure_node);
        break;
      } else {
        next = parent_failure_node;
      }
    }
  }

  for (size_t i = 0; i < node->GetChildren().size(); ++i) {
    Node *child = node->GetChildren()[i].next_;
    this->TranverseSetFailureNode(child);
  }
}

bool AcAutomata::Compile() {
  this->root_->SetFailureNode(NULL);
  this->TranverseSetFailureNode(this->root_);

  for (size_t i = 0; i < this->all_nodes_.size(); ++i) {
    Node *node = this->all_nodes_[i];
    node->SortEdges();
  }

  this->compiled_ = true;

  return true;
}

void AcAutomata::SetText(const std::string &text) {
  this->current_ = this->root_;
  this->text_ = text;
  this->position_ = 0;
}

bool AcAutomata::FindNext(MatchResult *result) {
  if (!this->compiled_) {
    return false;
  }
  if (this->text_.empty()) {
    return false;
  }

  if (result != NULL) {
    result->pattern_ = NULL;
    result->start_position_ = 0;
    result->end_position_ = 0;
  }

  Node *current = this->current_;
  size_t position = this->position_;
  bool found = false;

  while (position < this->text_.size()) {
    Node *next = current->FindNextBS(this->text_[position]);
    if (NULL == next) {
      if (current->GetFailureNode() != NULL) {
        // Jump to failure node.
        current = current->GetFailureNode();
      } else {
        // At root node
        ++position;
      }
    } else {
      // Continue Search
      current = next;
      ++position;
    }

    if (current->CheckFinal() && next != NULL) {
      if (result != NULL) {
        result->pattern_ = current->GetPattern();
        result->start_position_ = position - current->GetPattern()->size();
        result->end_position_ = position - 1;
      }
      found = true;
      break;
    }
  }

  this->current_ = current;
  this->position_ = position;

  if (found) {
    return true;
  } else {
    return false;
  }
}

}  // namespace global

