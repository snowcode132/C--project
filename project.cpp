#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <ctime>
#include <limits>
#include <algorithm>
using namespace std;

struct Point {
    int depression;
    string gender;
    int cluster;
    double minDist;

    Point() : depression(0), gender(""), cluster(-1), minDist(numeric_limits<double>::max()) {}

    Point(int depression, string gender) : depression(depression), gender(gender), cluster(-1), minDist(numeric_limits<double>::max()) {}

    double distance(Point p) {
        return (double)(depression - p.depression) * (depression - p.depression); // Simple distance using depression only
    }
};

vector<Point> readcsv(const string& filename) {
    vector<Point> points;
    ifstream file(filename);
    string line;

    // Skip comment lines starting with '%'
    while (getline(file, line)) {
        if (!line.empty() && line[0] != '%') break;
    }

    // Skip until we reach @DATA
    while (getline(file, line)) {
        if (line.find("@DATA") != string::npos) break;
    }

    // Now read actual data lines
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> row;

        while (getline(ss, cell, ',')) {
            cell.erase(remove(cell.begin(), cell.end(), '\''), cell.end());
            row.push_back(cell);
        }

        if (row.size() < 18) continue;

        try {
            string gender = row[1];
            int depression = stoi(row[17]);
            points.push_back(Point(depression, gender));
        } catch (...) {
            continue;
        }
    }

    file.close();
    return points;
}

void kMeansClustering(vector<Point>* points, int epochs, int k) {
    int n = points->size();
    vector<Point> centroids;

    srand(time(0));
    for (int i = 0; i < k; ++i) {
        centroids.push_back(points->at(rand() % n));
    }

    for (int e = 0; e < epochs; ++e) {
        // Assign clusters
        for (vector<Point>::iterator c = centroids.begin(); c != centroids.end(); ++c) {
            int clusterId = c - centroids.begin();

            for (vector<Point>::iterator it = points->begin(); it != points->end(); ++it) {
                Point p = *it;
                double dist = c->distance(p);
                if (dist < p.minDist) {
                    p.minDist = dist;
                    p.cluster = clusterId;
                }
                *it = p;
            }
        }

        // Prepare to calculate new centroids
        vector<int> nPoints(k, 0);
        vector<int> sumDepression(k, 0);

        for (vector<Point>::iterator it = points->begin(); it != points->end(); ++it) {
            int clusterId = it->cluster;
            nPoints[clusterId] += 1;
            sumDepression[clusterId] += it->depression;
            it->minDist = numeric_limits<double>::max();
        }

        for (vector<Point>::iterator c = centroids.begin(); c != centroids.end(); ++c) {
            int clusterId = c - centroids.begin();
            c->depression = sumDepression[clusterId] / nPoints[clusterId];
        }
    }

    // Save results
    ofstream myfile("output.txt");
    myfile << "depression,gender,cluster" << endl;
    for (vector<Point>::iterator it = points->begin(); it != points->end(); ++it) {
        myfile << it->depression << "," << it->gender << "," << it->cluster << endl;
    }
    myfile.close();
}

int main() {
    vector<Point> points = readcsv("OpenML Dataset");
    kMeansClustering(&points, 100, 5);
    cout << "Clustering complete. Results saved to output.csv." << endl;
    return 0;
}
