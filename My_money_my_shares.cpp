#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

void distributeApples(vector<int> &appleWeights);

int main()
{
    vector<int> appleWeights;
    int weight;

    cout << "Enter apple weights in grams (enter -1 to stop): " << endl;

    // Read weights from the console until -1 is entered
    while (true)
    {
        cin >> weight;
        if (weight == -1)
            break;                      // Stop when -1 is entered
        appleWeights.push_back(weight); // Add weight to the list
    }

    distributeApples(appleWeights);

    return 0;
}

void distributeApples(vector<int> &appleWeights)
{
    // Sort weights in descending order
    sort(appleWeights.begin(), appleWeights.end(), greater<int>());

    // Initialize lists for Ram, Sham, and Rahim
    vector<int> ram, sham, rahim;

    // Define the share of each person
    int ramShare = 50;   // 50% of total weight
    int shamShare = 30;  // 30% of total weight
    int rahimShare = 20; // 20% of total weight

    // Calculate total weight
    int totalWeight = accumulate(appleWeights.begin(), appleWeights.end(), 0);

    // Calculate individual weight limits based on total weight
    int ramLimit = (ramShare * totalWeight) / 100;
    int shamLimit = (shamShare * totalWeight) / 100;
    int rahimLimit = (rahimShare * totalWeight) / 100;

    // Initialize weight trackers
    int ramWeight = 0;
    int shamWeight = 0;
    int rahimWeight = 0;

    // Distribute apples according to share limits
    for (int weight : appleWeights)
    {
        if (ramWeight + weight <= ramLimit)
        {
            ram.push_back(weight);
            ramWeight += weight; // Add weight to Ram's total
        }
        else if (shamWeight + weight <= shamLimit)
        {
            sham.push_back(weight);
            shamWeight += weight; // Add weight to Sham's total
        }
        else if (rahimWeight + weight <= rahimLimit)
        {
            rahim.push_back(weight);
            rahimWeight += weight; // Add weight to Rahim's total
        }
    }

    // Output the distributed apples
    cout << "Ram: ";
    for (int w : ram)
        cout << w << " ";
    cout << "| Total Weight: " << ramWeight << "g" << endl;

    cout << "Sham: ";
    for (int w : sham)
        cout << w << " ";
    cout << "| Total Weight: " << shamWeight << "g" << endl;

    cout << "Rahim: ";
    for (int w : rahim)
        cout << w << " ";
    cout << "| Total Weight: " << rahimWeight << "g" << endl;
}