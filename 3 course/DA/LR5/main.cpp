#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;




#define SENTINEL '~'

class SuffixTreeUkkonen;

class Node {
public:

    friend SuffixTreeUkkonen;

    Node(Node * suffix_link, int first, int * last)
        : Node(suffix_link, first, last, false)
    {}

    Node(Node * suffix_link, int first, int * last, bool end)
        : suffix_link_(suffix_link)
        , first_(first)
        , last_(last)
        , end_(end)
    {}

    bool IsLeaf() { return end_; }

    Node * GetMinChild() { return children_.begin()->second; }

    int GetFirstIdx() { return first_; }
    int GetLastIdx() { return *last_; }

private:

    map<char, Node *> children_;
    Node * suffix_link_;

    int first_;
    int * last_;
    bool end_;

};




class SuffixTreeUkkonen {
public:

    SuffixTreeUkkonen(const string & text)
        : text_(text + SENTINEL) 
    {
        BuildSuffixTree();
        // PrintSuffixTree(root_);
    }

    ~SuffixTreeUkkonen() { DeleteSuffixTree(root_); }

    void PrintSuffixTree(Node * node, int level = -1) {

        if (node != root_) {

            for (int i = 0; i < level; ++i)
                cout << "\t";

            for (int i = node->first_; i < *node->last_ + 1; ++i)
                cout << text_[i];
            cout << endl;

        }

        for (const auto & child : node->children_)
            PrintSuffixTree(child.second, level + 1);

    }

    Node * GetRoot() { return root_; }

private:

    void BuildSuffixTree() {

        current_node_ = root_;
        for (size_t i = 0; i < text_.length(); ++i)
            InsertLetter(i);
        
    }

    void InsertLetter(int idx) {

        ++global_end_;
        ++number_uninserted_suffixes_;
        last_created_internal_node_ = nullptr;

        while (number_uninserted_suffixes_ > 0) {

            if (current_length_ == 0) current_edge_ = idx;

            auto find = current_node_->children_.find(text_[current_edge_]);

            if (find == current_node_->children_.end()) {

                current_node_->children_.insert(make_pair(text_[current_edge_], new Node(root_, idx, &global_end_, true)));

                if (last_created_internal_node_ != nullptr) {
                    last_created_internal_node_->suffix_link_ = current_node_;
                    last_created_internal_node_ = nullptr;
                }

            } else {

                Node * next_node = find->second;
                int edge_length = *(next_node->last_) - (next_node->first_) + 1;

                if (current_length_ >= edge_length) {
                    current_edge_ += edge_length;
                    current_length_ -= edge_length;
                    current_node_ = next_node;
                    continue;
                }

                if (text_[next_node->first_ + current_length_] == text_[idx]) {
                    if (last_created_internal_node_ != nullptr && current_node_ != root_)
                        last_created_internal_node_->suffix_link_ = current_node_;
                    ++current_length_;
                    break;
                }

                Node * split = new Node(root_, next_node->first_, new int(next_node->first_ + current_length_ - 1));

                current_node_->children_[text_[current_edge_]] = split;

                next_node->first_ += current_length_;

                split->children_.insert(make_pair(text_[next_node->first_], next_node));

                split->children_.insert(make_pair(text_[idx], new Node(root_, idx, &global_end_, true)));

                if (last_created_internal_node_ != nullptr)
                    last_created_internal_node_->suffix_link_ = split;

                last_created_internal_node_ = split;

            }

            --number_uninserted_suffixes_;

            if (current_node_ == root_ && current_length_ > 0) {
                --current_length_;
                ++current_edge_;
            } else if (current_node_ != root_)
                current_node_ = current_node_->suffix_link_;

        }

    }

    void DeleteSuffixTree(Node * node) {

        for (auto child : node->children_)
            DeleteSuffixTree(child.second);

        if (!node->end_)
            delete node->last_;

        delete node;

    }

    string text_;

    Node * root_ = new Node(nullptr, -1, new int(-1));

    Node * last_created_internal_node_ = nullptr;
    Node * current_node_ = nullptr;
    
    int global_end_ = -1;
    int current_edge_ = -1;
    int current_length_ = 0;
    int number_uninserted_suffixes_ = 0;

};




class LexicographicalMinCut {
public:

    static string findMinCut(const string & text) {

        LexicographicalMinCut cutter(text);

        return cutter.findMinCut();

    }
    
private:

    LexicographicalMinCut(const string & text)
        : text_(text + text + SENTINEL)
        , text_legnth_(text.length())
        , suffix_tree_(text + text)
    {}

    string findMinCut() {

        string min_cut = "";

        Node * node = suffix_tree_.GetRoot();

        while (!node->IsLeaf()) {
            node = node->GetMinChild();            
            min_cut += text_.substr(node->GetFirstIdx(), node->GetLastIdx() - node->GetFirstIdx() + 1);
        }

        return min_cut.substr(0, text_legnth_);

    }

    string text_;
    int text_legnth_;
    SuffixTreeUkkonen suffix_tree_;

};




int main() {

    string text, min_cut_text;
    cin >> text;

    min_cut_text = LexicographicalMinCut::findMinCut(text);

    cout << min_cut_text << endl;

    return 0;

}
