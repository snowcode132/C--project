#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <ctime>
#include <limits>
#include <algorithm>
#include <iomanip>
using namespace std;

//this is a structure that we will use to make the points that we will graph
struct Point {
    double studyTime;
    double grade;
    int cluster;
    double minDist;

    Point() : studyTime(0.0), grade(0.0), cluster(-1), minDist(numeric_limits<double>::max()) {}

    Point(double studyTime, double grade) : studyTime(studyTime), grade(grade), cluster(-1), minDist(numeric_limits<double>::max()) {}

    double distance(Point p) {
        return pow(studyTime - p.studyTime, 2) + pow(grade - p.grade, 2);
    }
};

//this code grabs the information from the data set that we will be using and puts it into a vector
vector<Point> readcsv(const string& filename) {
    vector<Point> points;
    ifstream file(filename); //open the dataset fil 
    string line;

    // Skip header
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> row;

        while (getline(ss, cell, ',')) {
            cell.erase(remove(cell.begin(), cell.end(), '\''), cell.end());
            row.push_back(cell);
        }

        if (row.size() < 14) continue; 

        try {
            double studyTime = stod(row[5]);  //study time
            double grade = stod(row[13]);     //gpa
            points.push_back(Point(studyTime, grade));
        } catch (...) {
            continue;
        }
    }

    file.close(); //close the file
    return points;
}


//this is the function where we will calculate centroids with the information given to us from the dataset
void kMeansClustering(vector<Point>* points, int epochs, int k) {
    int n = (*points).size();
    vector<Point> centroids;

    srand(time(NULL));
    for (int i = 0; i < k; ++i) {
        centroids.push_back((*points)[rand() % n]
    );
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

        // Recalculate centroids
        vector<int> nPoints(k, 0);
        vector<double> sumStudy(k, 0.0);
        vector<double> sumGrade(k, 0.0);

        for (vector<Point>::iterator it = points->begin(); it != points->end(); ++it) {
            int clusterId = it->cluster;
            nPoints[clusterId]++;
            sumStudy[clusterId] += it->studyTime;
            sumGrade[clusterId] += it->grade;
            it->minDist = numeric_limits<double>::max();
        }

        for (vector<Point>::iterator c = centroids.begin(); c != centroids.end(); ++c) {
            int clusterId = c - centroids.begin();
            if (nPoints[clusterId] > 0) {
                c->studyTime = sumStudy[clusterId] / nPoints[clusterId];
                c->grade = sumGrade[clusterId] / nPoints[clusterId];
            }
        }
    }

    // Output results to output.txt
    ofstream myfile("output.txt");
    myfile << fixed << setprecision(4);
    myfile << "studyTime,grade,cluster" << endl;
    for (vector<Point>::iterator it = points->begin(); it != points->end(); ++it) {
        myfile << it->studyTime << "," << it->grade << "," << it->cluster << endl;
    }
    myfile.close();
}

int main() {
    vector<Point> points = readcsv("Student_performance_data");  
    kMeansClustering(&points, 100, 5);
    cout << "✅ Clustering complete. Results saved to output.txt." << endl;
    return 0;
}

