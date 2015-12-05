#ifndef __GLOBAL__AC__AUTOMATA__H
#define __GLOBAL__AC__AUTOMATA__H

#include <list>
#include <string>
#include <vector>

namespace global {

class AcAutomata {
 public:
  struct MatchResult {
    const std::string *pattern_;
    size_t start_position_;
    size_t end_position_;
  };

  AcAutomata();
  ~AcAutomata();

  bool AddPattern(const std::string &pattern);
  bool Compile();
  void SetText(const std::string &text);
  bool FindNext(MatchResult *result);

 private:
  // Edge Type
  class Node;

  struct Edge {
    char alpha_;
    Node *next_;
  };

  struct EdgeLess {
    bool operator()(const Edge &lhs, const Edge &rhs);
  };

  // Node Type
  class Node {
   public:
    Node();
    ~Node();

    bool CheckFinal() const { return this->final_; }
    void SetFinal() { this->final_ = true; }
    const std::string *GetPattern() { return this->pattern_; }
    Node *GetFailureNode() { return this->failure_node_; }
    void SetFailureNode(Node *node) { this->failure_node_ = node; }
    Node *GetParentNode() { return this->parent_; }
    const std::vector<Edge> &GetChildren() { return this->children_; } 

    Node *CreateNext(char alpha);
    void RegisterPattern(const std::string *pattern);
    void SortEdges();
    Node *FindNext(char alpha);
    Node *FindNextBS(char alpha);

   private:
    int depth_;
    bool final_;
    const std::string * pattern_;
    Node *failure_node_;
    Node *parent_;
    std::vector<Edge> children_;
  };

 private:
  void TranverseSetFailureNode(Node *node);

  std::list<std::string> patterns_store_;
  Node *root_;
  std::vector<Node *> all_nodes_;
  bool compiled_;

  Node *current_;
  std::string text_;
  size_t position_;
};

}  // namespace global

#endif  // __GLOBAL__AC__AUTOMATA__H

