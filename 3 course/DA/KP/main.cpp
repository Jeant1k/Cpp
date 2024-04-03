#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <memory>
#include <chrono>

struct Point {
    std::vector<int> coordinates;
    int index;
};

class KDTree {
private:
    struct Node {
        Point point;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
    };

    std::unique_ptr<Node> root;
    int dimension;

    std::unique_ptr<Node> build(std::vector<Point>& points, int depth, int start, int end) {
        if (start >= end) return nullptr;

        int axis = depth % dimension;
        std::sort(points.begin() + start, points.begin() + end, [axis](const Point& a, const Point& b) {
            return a.coordinates[axis] < b.coordinates[axis];
        });

        int mid = start + (end - start) / 2;
        while (mid < end - 1 && points[mid].coordinates[axis] == points[mid + 1].coordinates[axis])
            ++mid;

        auto node = std::make_unique<Node>();
        node->point = points[mid];
        node->left = build(points, depth + 1, start, mid);
        node->right = build(points, depth + 1, mid + 1, end);

        return node;
    }

    double squaredDistance(const Point& a, const Point& b) {
        double dist = 0;
        for (int i = 0; i < dimension; ++i)
            dist += pow(a.coordinates[i] - b.coordinates[i], 2);
        return dist;
    }

    void nearestNeighbor(Node* node, const Point& point, int depth, Point& best, double& bestDist) {
        if (!node) return;

        double d = squaredDistance(node->point, point);
        if (d < bestDist) {
            best = node->point;
            bestDist = d;
        }

        int axis = depth % dimension;
        Node* first = node->left.get();
        Node* second = node->right.get();

        if (point.coordinates[axis] > node->point.coordinates[axis]) std::swap(first, second);

        nearestNeighbor(first, point, depth + 1, best, bestDist);
        if (pow(point.coordinates[axis] - node->point.coordinates[axis], 2) < bestDist)
            nearestNeighbor(second, point, depth + 1, best, bestDist);
    }

public:
    KDTree(std::vector<Point>& points) : dimension(points[0].coordinates.size()) {
        root = build(points, 0, 0, points.size());
    }

    int nearestNeighbor(const Point& point) {
        Point best = root->point;
        double bestDist = squaredDistance(root->point, point);
        nearestNeighbor(root.get(), point, 0, best, bestDist);
        return best.index + 1;
    }

};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    setvbuf(stdin, NULL, _IOFBF, 4096);
    setvbuf(stdout, NULL, _IOFBF, 4096);

    int n, d;
    std::cin >> n >> d;

    std::vector<Point> points(n);
    for (int i = 0; i < n; ++i) {
        points[i].coordinates.resize(d);
        for (int j = 0; j < d; j++) std::cin >> points[i].coordinates[j];
        points[i].index = i;
    }

    KDTree tree(points);

    int q;
    std::cin >> q;
    while (q--) {
        Point query;
        query.coordinates.resize(d);
        for (int i = 0; i < d; ++i) std::cin >> query.coordinates[i];
        std::cout << tree.nearestNeighbor(query) << std::endl;
    }

    return 0;
}
