#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <chrono>

using namespace std;

 // fixed << setprecision(1) << (ans * 100.0) << "%" 

 // distance = sqrt (sum (object - neighbor) ^ 2) for each subset
 double distanceSub (const vector<double>& a, const vector<double>& b, const vector<int>& subset) {
    double answer = 0.0;
    for (int i : subset) {
        double difference = a[i] - b[i];
        answer+= difference * difference;
    }
    // returns squared distance
    return answer;
}

// Read in data from the file that the user enters
vector<vector<double>> readIn(const string& filename) {
    ifstream fileIn(filename);
    // error and exit if there is a problem opening the file
    if(!fileIn.is_open()) {
        cout << "Error: Cannot open file, try again please." << endl;
        exit(1);
    }
    // read in data
    vector<vector<double>> data;
    string line;
    while (getline(fileIn, line)) {
        stringstream in(line);
        vector<double> row;
        double value;
        while (in >> value) {
            row.push_back(value);
        }
        if (!row.empty()) {
            data.push_back(row);
        }

    }
    return data;
}

// Finds the accuracy for a subset of features
// similar to leave one out cross validation to notes
// computes each instance using nearest neighbor
// function accuracy = leave_one_out_cross_validation (data, current_set, feature_to_add)
// accuracy = rand; \\ this is a testing stub only


// Locate nearest neighbor and location (from notes)
// function accuracy = cs170demo()
// data = load (CS170_SMALLtestdata_1.txt);

//     for i = 1 : size(data,1)
//         object_to_classify = data(i,2:end);
//         label_object_to_classify = data(i,1);

//         nearest_neighbor_distance = inf;
//         nearest_neighbor_location = inf;

//         for k = 1 : size(data, 1)
//             if k ~= i // dont compare to yourself!!!!
//                 disp(["Ask if ", int2str(i), " is the nearest neighbor with ", int2str(k)])
//             end
//         end
//     end
// end

double leaveOneOut(const vector<vector<double>>& data, const vector<int>& subset) {
    int numofcorrect = 0;
    double nearestDistance, nearestLabel;

    int size = static_cast<int>(data.size());

    for(int i = 0; i < size; ++i) {
        const vector<double>& objectToClassify = data[i];
        double labelObjectToClassify = objectToClassify[0];

        nearestDistance = numeric_limits<double>::infinity();
        nearestLabel = -1.0;

        // search for all other instances
        for(int k = 0; k < size; ++k) {
            // skip itself
            if (k == i) {
                continue;
            }
            double distance = distanceSub(objectToClassify, data[k], subset);
            if (distance < nearestDistance) {
                nearestDistance = distance;
                // The class is the first column in the text file
                nearestLabel = data[k][0];
            }
        }
        if(labelObjectToClassify == nearestLabel) {
            numofcorrect++;
        }

    }  
    return static_cast<double> (numofcorrect) / size ;  
} 

// forward selection 
// function accuracy = cs170demo()
// data = load (CS170_SMALLtestdata_1.txt);
// number_correctly_classified = 0;

//     for i = 1 : size(data,1)
//         object_to_classify = data(i,2:end);
//         label_object_to_classify = data(i,1);

//         nearest_neighbor_distance = inf;
//         nearest_neighbor_location = inf;

//         for k = 1 : size(data, 1)
//             disp(["Ask if ", int2str(i), " is the nearest neighbor with ", int2str(k)])

//             if k ~= i
//             distance = sqrt(sum((object_to_classify - data(k,2:end)).^2));
//                     if distance < nearest_neighbor_distance
//                         nearest_neighbor_distance = distance;
//                         nearest_neighbor_location = k;
//                         nearest_neighbor_label = data(nearest_neighbor_location,1);
//                     end
                
//             end
//         end
//         if label_object_to_classify == nearest_neighbor_label;
//             number_correctly_classified = number_correctly_classified + 1;
//         end
//     end
//     accuracy = number_correctly_classified / size(data,1);
// end

// // This function returns 0.4000

// Similar to example feature search demo
vector<int> forwardSelection(const vector<vector<double>>& data) {
    // total features = size(data, 2) - 1
    //data[i][0] is the label
    // columns from 1 to the end of the file are features
    int totalFeatures = static_cast<int>(data[0].size()) - 1;

    // current set of features and best set of features overall
    vector<int> currSet, bestSetOverall;
    double BetteraccuracyOverall = 0.0;

    // for i = 1 : size(data,2) - 1
    // disp["on the ",  num2str(i), "th level "]
    for (int level = 1; level <= totalFeatures; ++level) {
        cout << "On level " << level << " of the search tree ---------------" << endl;
        // which feature to add at the current level and accuracy set to 0
        int feature2Add = -1;
    double bestAccSoFar = 0.0;
        // for k = 1 : size(data,2) - 1
        // if k is not in current set of features...
        for(int currFeature = 1; currFeature <= totalFeatures; ++currFeature) {
            // already in current set of features
            bool alreadyincluded = (find(currSet.begin(), currSet.end(), currFeature) != currSet.end());
            if (!alreadyincluded) {
                // disp["--Considering adding the ", num2str(k), "feature"]) from notes
                vector<int> temp = currSet;
                temp.push_back(currFeature);
                // accuracy = leave one out validation
                double accuracy = leaveOneOut(data, temp);
                // output the accuracy for each feature
                cout << "--Considering adding feature " << currFeature << " Accuracy: " << fixed << setprecision(1) << (accuracy * 100.0) << endl;
                // if the accuracy is better than the accuracy so far..
                if (accuracy > bestAccSoFar) {
                    bestAccSoFar = accuracy;
                    feature2Add = currFeature;
                }
            }
        }
        // Pick the best accuracy out of all the features 
        if (feature2Add != -1) {
            currSet.push_back(feature2Add);
            cout << "On level " << level << ", I added feature " << feature2Add << " to current set" << endl;
            cout << "Accuracy: " << fixed << setprecision(1) << (bestAccSoFar * 100.0) << endl;

            // track if this is the best overall accuracy or not
            if (bestAccSoFar > BetteraccuracyOverall) {
                BetteraccuracyOverall = bestAccSoFar;
                bestSetOverall = currSet;
            }
        }
    }
    // output the best subset and the best overall accuracy
    cout << "----------------------------------------------------------------" << endl;
    cout << "Best subset found by forward selection: { ";
    for (int i : bestSetOverall)
    cout << i << " ";
    cout << "}, with accuracy:" << fixed << setprecision(1) << (BetteraccuracyOverall * 100.0) << "%" << endl;
    return bestSetOverall;
}
// Backward elimination (similar to Forward Selection)
vector<int> backwardElimination(const vector<vector<double>>& data) {
    // total features = size(data, 2) - 1
    //data[i][0] is the label
    // columns from 1 to the end of the file are features
    int Features = static_cast<int> (data[0].size()) - 1;

    // current set of features
    vector<int> currset;
    for(int i = 1; i <= Features; ++i) {
        currset.push_back(i);
    }

    // compute the full set of features
    double bestAccOverall = leaveOneOut(data, currset);

    vector<int> bestSetOverall = currset;
    // Print out the accuracy of the current features
    cout << "The initial set accuracy is: " << fixed << setprecision(1) << (bestAccOverall * 100.0) << "%" << endl;

    // This loop will remove a feature from each level
    // this will continue until it is finished
    for(int level = 1; level < Features; ++level) {
        cout << "On level " << level << " of the search tree" << endl;
        // which feature to remove and the accuracy
        int feat2Remove = -1;
        double betterAccnow = 0.0;
        
        
        // consider which features to remove at each level
        for(int feat : currset) {
            vector<int> temp;
            for(int i : currset) {
                if (i != feat) {
                    temp.push_back(i);
                }
            }
            // accuracy = leave one out 
            double acc = leaveOneOut(data, temp);
            cout << "--Considering removing feature " << feat << " Accuracy: " << fixed << setprecision(1) << (acc * 100.0) << "%" << endl;
            // if accuracy is better than the accuracy now
            if (acc > betterAccnow) {
                betterAccnow = acc;
                feat2Remove = feat;
            }
        }
        // check all the levels and remove the best feature
        if (feat2Remove != -1) {
            vector<int> NewerSet;

            for(int i : currset) {
                if (i != feat2Remove) {
                    NewerSet.push_back(i);
                }
            }
            // output data from which feature to remove from the current set on which level
            currset = NewerSet;
            cout << "On level " << level << ", I removed feature " << feat2Remove << " from current set" << endl;
            cout << "Accuracy: " << fixed << setprecision(1) << (betterAccnow * 100.0) << "%" << endl;
            cout << endl;

            if (betterAccnow > bestAccOverall) {
                bestAccOverall = betterAccnow;
                bestSetOverall = currset;
            }
        }
    }
    // After we finish computing the data
    // output the best subset found by backward elimination
    // set precision to output as percentage
    cout << "Best subset found by Backward Elimination: { ";
    for(int i : bestSetOverall) 
    cout << i << " ";
    cout << "}, with accuracy: " << fixed << setprecision(1) << (bestAccOverall * 100.0) << "%" << endl;
    return bestSetOverall;
}


int main() {
    cout << "Welcome, This program uses Forward Elimination or Backward Elimination to find the best subset of features " << endl;
    cout << "Type in the name of the file to test: (e.g. CS170_Large_Data__12.txt): " << endl;
    cout << endl;
    string filename;
    cin >> filename;
    cout << endl;
    cout << endl;

    // read in the file
    vector <vector<double>> data = readIn(filename);


// calculate the # of instances and features
int numofInstances = static_cast<int>(data.size());
int numofFeatures = 0;
if (numofInstances > 0) {
    numofFeatures = static_cast<int>(data[0].size()) - 1;
}

// This builds a list of the features 
vector<int>feats;
for(int i = 1; i <= numofFeatures; ++i) {
    feats.push_back(i);
}

// leave one out 
double ans = 0.0;
if(numofFeatures > 0) {
    ans = leaveOneOut(data, feats);
}
// output num of features and num of instances 
cout << "This dataset has " << numofFeatures << " features (not including the class attribute), " <<
" with " << numofInstances << " instances." << endl;
// output nearest neighbor 
cout << "Running nearest neighbor with all " << numofFeatures << " features, using leaving-one-out evaluation, i get an accuracy of " 
<< fixed << setprecision(1) << (ans * 100.0) << "%" << endl << endl;

cout << "Select which algorithm to run: " << endl;
cout << "[1] Forward Selection. " << endl;
cout << "[2] Backward Elimination" << endl;

int pChoice = 0;
    cin >>pChoice;
    // input validation
    // loops until choice is false
    while (true) {
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>:: max(), '\n');
            cout << "You have entered an invalid choice" << endl;
            cin >> pChoice;
        }
        if (!cin.fail())
        break;
    }
    // loop to get user to choose from the following options
    while (pChoice != 1 && pChoice != 2) {
        cin.clear();
        cout << "Invalid choice, please try again!" << endl;
        cin >> pChoice;
    }
    if (pChoice == 1) {
    // Forward Selection 
    // time starts and ends
    clock_t start;
    start = clock();
    cout << "Starting with Forward selection..." << endl;
    vector<int> Forward = forwardSelection(data);
    clock_t end;
    end = clock();
    // Make time in MS more readable
    double time = double(end - start) / CLOCKS_PER_SEC ;
    cout << "Forward Selection finished in: " << time << " seconds" << endl;
    }


    if (pChoice == 2) {
        // Backward Elimination
    clock_t start;
    start = clock();
    cout << "Starting with Backward Elimination..." << endl;
    vector<int> bestBackward = backwardElimination(data);
    clock_t end;
    end = clock();
    // Make time in MS more readable
    double time = double(end - start) / CLOCKS_PER_SEC;
    cout << "Backward Elimination finished in: " << time << " seconds" << endl;
    }

    return 0;
}