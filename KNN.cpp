#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <unordered_map>
#include <sstream>
using namespace std;

// Struct de luu tru du lieu cua moi diem
struct Point {
    int id;
    vector<double> features;
    string label;
    double distance;
};

// Tinh khoang cach Euclid giua hai diem
double calcDistance(const vector<double>& pointA, const vector<double>& pointB) {
    double sumSquaredDiffs = 0;
    for (size_t i = 0; i < pointA.size(); i++) {
        double diff = pointA[i] - pointB[i];
        sumSquaredDiffs += diff * diff;
    }
    return sqrt(sumSquaredDiffs);
}

// Tim nhan xuat hien nhieu nhat trong k hang xom gan nhat
string findMostOccur(const vector<string>& labels) {
    unordered_map<string, int> labelCount;
    string mostFrequentLabel;
    int maxCount = 0;

    for (const auto& label : labels) {
        if (++labelCount[label] > maxCount) {
            maxCount = labelCount[label];
            mostFrequentLabel = label;
        }
    }
    return mostFrequentLabel;
}

// Dinh dang gia tri dac trung
string formatFeature(double feature) {
    if (feature == static_cast<int>(feature))
        return to_string(static_cast<int>(feature));
    stringstream stream;
    stream << fixed << setprecision(4) << feature;
    return stream.str();
}

int main() {
    ifstream inFile("test.txt");
    if (!inFile) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    vector<Point> trainSet;
    Point testPoint;
    double x, y;
    string label;
    int idCounter = 1;

    // Doc du lieu vao tap huan luyen, dong cuoi cung la diem kiem tra
    while (inFile >> x >> y) {
        Point p = { idCounter++, {x, y}, "" };
        if (!(inFile >> label)) {
            testPoint = p;
            break;
        }
        p.label = label;
        trainSet.push_back(p);
    }

    int k = 3; // So luong hang xom gan nhat
    vector<Point> distances;

    // Tinh khoang cach tu moi diem huan luyen den diem kiem tra
    for (const auto& trainPoint : trainSet) {
        Point neighbor = trainPoint;
        neighbor.distance = calcDistance(testPoint.features, trainPoint.features);
        distances.push_back(neighbor);
    }

    // Sap xep khoang cach tang dan
    sort(distances.begin(), distances.end(), [](const Point &a, const Point &b) {
        return a.distance < b.distance || (a.distance == b.distance && a.id < b.id);
    });

    // Lay nhan cua k hang xom gan nhat va du doan nhan cho diem kiem tra
    vector<string> nearestLabels;
    for (int j = 0; j < k && j < distances.size(); j++)
        nearestLabels.push_back(distances[j].label);
    string prediction = findMostOccur(nearestLabels);

    // Xuat ket qua du doan va ba hang xom gan nhat
    cout << "Du doan cho diem kiem tra (" << testPoint.features[0] << ", " << testPoint.features[1] << "): " << prediction << "\n\n";
    cout << "Ba hang xom gan nhat:\n";
    for (int j = 0; j < k && j < distances.size(); j++) {
        cout << "(ID: " << distances[j].id << "): "
             << "Dac trung = [" << formatFeature(distances[j].features[0]) << ", " << formatFeature(distances[j].features[1]) << "], "
             << "Nhan = " << distances[j].label << ", "
             << "Khoang cach = " << fixed << setprecision(2) << distances[j].distance << "\n";
    }

    // Hien thi bang du lieu sau khi tinh khoang cach va sap xep
    cout << "\nBang du lieu sau khi tinh khoang cach va sap xep:\n";
    cout << "ID   Features       Label     Distance\n";
    cout << "=======================================================\n";
    for (const auto& neighbor : distances) {
        cout << setw(2) << neighbor.id << "   "
             << formatFeature(neighbor.features[0]) << ", " << formatFeature(neighbor.features[1]) << "          "
             << neighbor.label << "         "
             << fixed << setprecision(2) << neighbor.distance << "\n";
    }
    return 0;
}

