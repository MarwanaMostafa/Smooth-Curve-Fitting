#include <iostream>
#include <bits/stdc++.h>
using namespace std;
#define POPULATION_SIZE 100
#define PC 0.7
#define PM 0.01
#define LBOUND -10
#define UBOUND 10
#define NUMBEROFITERATIONS 100

void initializePopulation(vector<vector<float>> &pop, int sizegene) // size of gene represent flag for each item so numberOfItems=sizeOfGene
{
    int sizeGene = sizegene;
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        vector<float> temp;
        for (int j = 0; j < sizeGene; j++)
            temp.push_back(((float(rand()) / float(RAND_MAX)) * (20.0)) - 10.0); // why *20 because we need create random number with specific range so we applay equation =>(high-low)+low and hence high is 10 and low is -10
        pop.push_back(temp);
    }
}
void calculateFitness(vector<float> &Fitness, vector<pair<float, float>> points, vector<vector<float>> Pop)
{
    float summationVal = 0;
    for (int i = 0; i < Pop.size(); i++)
    {
        for (int j = 0; j < points.size(); j++)
        {
            float tempSummation = Pop[i][0]; // add first coefficient (اللى مش بيبقى جمبه اكسات ده اللى لوحده)
            for (int k = 1; k < Pop[0].size(); k++)
                // cout << Pop[i][k] << " " << points[j].first << endl;
                tempSummation += (pow(points[j].first, k) * Pop[i][k]); //هنا انا بحل المعادلة فى الاكس يعنى لو معايا عشر نقط فانا هنا بمسك نقطة نقطة واعوض بيها بس بعوض بالاكسات
            summationVal += pow(tempSummation - points[j].second, 2);   //انما هنا بقى انا خلص عوض بالاكسات فدلوقتى المفروض اطرح من الناتج اللى طلعلى الواى واربعهم
        }
        // why we push 1/error because when we perform selection, we usually  select the “fitter” chromosomes fitter hence =less error so we divide by 1 to take best rank
        Fitness.push_back(1 / (summationVal / points.size())); //هنا بعد مجمعت كل النقط بقسم على عددهم بقىTOTAL ERROR
        summationVal = 0;
    }
}
// Each element in FITNESS corresponds to it in the same order as the chromosome in POP
//يعنى انا دلوقتى مثلا مثلا هشوف اول عنصر وتانى عنصر وهختار منهم واحد لو اختارت الاول يبقى ده كده معناه انى هاخد اول عنصر فى اراى ال
// POP
void tournamentSelection(vector<float> Fitness, set<int> &selectedVector)
{

    //الفكرة كلها من الكونديشن ده بسبب ان ممكن يطلعلى نفس الرقم العشوائى مرتين ويكون هو احسن واحد برضو فساعتها هختاروا مرتين
    // فعلشان كده عملتها سيت السيت مش هيخرج من اللوب غير لما يبقى عدد الاختيارات نص اللى موجود
    // هتقولى مهو ممكن اعمل رقمين عشوائين بدل من انى بعمل رقم واحد واجيب اللى بعدو ممكن هقلك هى هى برضو لان ممكن رقم من ضمن الرقمين يتكرر مرتين وكمان يكون هو ليه احسن فيتنس فساعتها هيتاخد مرتين
    for (int i = 0; selectedVector.size() != Fitness.size() / 2; i++)
    {
        // cout << Fitness[i] << endl;
        int x = rand() % (Fitness.size() - 2); //علشان ميجبليش رقم برا الرينج طيب ليه ناقص اتنين علشان لو عندى عشر عناصر فهيبقى الطول بتاعها عشرة بس مرقمة من زيرو لتسعة فانا لو قلت ناقص
                                               //  واحد بدل اتنين ممكن يطلعلى رقم تسعة مثلا ولما اجى فى السطر اللى جاى اقولو هاتلى الفيتنس اللى رقمه تسعة زاد واحد ساعتها هيضرب ايرور
        // cout << x << endl;
        // cout << Fitness[x + 1] << endl;
        if (Fitness[x] < Fitness[x + 1])
            selectedVector.insert(x + 1);
        else
            selectedVector.insert(x);
    }
}
void mutation(int numberOfIteration, vector<vector<float>> &mutationChromosomes, int index1, int index2)
{
    double y, deltaL, deltaU, amounfOfMutation;
    int i = index1;
    for (int counter = 0; counter < 2; counter++) // offspring which produce from crossover(2offsprings)
    {
        for (int j = 0; j < mutationChromosomes[0].size(); j++)
        {
            float r1 = (float)(rand() % RAND_MAX);
            deltaL = mutationChromosomes[i][j] - LBOUND;
            deltaU = UBOUND - mutationChromosomes[i][j];

            if (r1 > 0.5)
                y = deltaU;
            else
                y = deltaL;
            amounfOfMutation = y * (1 - pow(r1, pow(1 - (numberOfIteration / NUMBEROFITERATIONS), 1))); // doctor said assume b=1
            float r2 = (float)(rand() % RAND_MAX);
            if (r2 <= PM)
            {
                if (r1 > 0.5)
                    mutationChromosomes[i][j] += amounfOfMutation;
                else
                    mutationChromosomes[i][j] -= amounfOfMutation;
            }
        }
        i = index2;
    }
}

void crossOverAndMutation(int t, vector<vector<float>> &reproductionChromosomes)
{
    int tempCuttingPoint1 = rand() % (reproductionChromosomes[0].size());
    int tempCuttingPoint2 = rand() % (reproductionChromosomes[0].size());
    int cuttingPoint1, cuttingPoint2 = 0;
    if (tempCuttingPoint1 > tempCuttingPoint2)
    {
        cuttingPoint1 = tempCuttingPoint2;
        cuttingPoint2 = tempCuttingPoint1;
    }
    else
    {
        cuttingPoint1 = tempCuttingPoint1;
        cuttingPoint2 = tempCuttingPoint2;
    }
    if (cuttingPoint1 == cuttingPoint2)
        cuttingPoint2 = cuttingPoint1 + rand() % (reproductionChromosomes[0].size() - cuttingPoint1);
    for (int i = 0; i < reproductionChromosomes.size(); i += 2)
    {
        float r2 = (float)rand() / RAND_MAX;
        if (r2 < PC)
        {
            for (int j = cuttingPoint1; j <= cuttingPoint2; j++)
            {
                float temp = reproductionChromosomes[i][j];
                reproductionChromosomes[i][j] = reproductionChromosomes[(i + 1) % reproductionChromosomes.size()][j];
                reproductionChromosomes[(i + 1) % reproductionChromosomes.size()][j] = temp;
            }
            mutation(t, reproductionChromosomes, i, (i + 1) % reproductionChromosomes.size());
        }
    }
}
void elitistReplacement(vector<float> FitnessParents, vector<float> FitnessOffSprings, set<int> indexSelectedParent, vector<vector<float>> &pop, vector<vector<float>> offSprings)
{
    int counter = 0;
    for (auto i : indexSelectedParent) // auto i because this is set
    {
        if (FitnessOffSprings[counter] > FitnessParents[i])
            pop[i] = offSprings[counter];
        counter++;
    }
}
void readPoints(int numberOfPoints, ifstream &ReadFile, vector<pair<float, float>> &Points)
{
    string readLine;
    for (int i = 0; i < numberOfPoints; i++)
    {
        getline(ReadFile, readLine);
        int pos = readLine.find(" ");
        Points.push_back({stof(readLine.substr(0, pos)), stof(readLine.substr(pos + 1))});
    }
    // for (int i = 0; i < numberOfPoints; i++)
    // cout << Points[i].first << "   " << Points[i].second << endl;
}
void writeOptimalSoultion(ofstream &outputFile, vector<float> Fitness, vector<vector<float>> pop)
{
    int indexMaxFitness = 0;
    for (int i = 1; i < Fitness.size(); i++)
        if (Fitness[i] > Fitness[indexMaxFitness]) // max fitness min error
            indexMaxFitness = i;
    outputFile << "Mean Square Error Is : " << Fitness[indexMaxFitness] << endl;
    outputFile << "coefficients of the polynomial function Is : " << endl;
    for (int i = 0; i < pop[0].size(); i++)
        outputFile << pop[indexMaxFitness][i] << endl;
}
int main()
{
    srand(time(NULL));

    ofstream MyFile("Output.txt");
    ifstream MyReadFile("Input.txt");
    string readLine = "";
    getline(MyReadFile, readLine);
    int numberOfTestCase = stoi(readLine);

    for (int i = 0; i < numberOfTestCase; i++)
    {
        getline(MyReadFile, readLine);

        int numberOfPoints = stoi(readLine.substr(0, readLine.find(" ")));
        int lengthOfGene = stoi(readLine.substr(readLine.find(" ") + 1));
        vector<pair<float, float>> Points;
        readPoints(numberOfPoints, MyReadFile, Points);
        vector<vector<float>> pop;
        vector<float> Fitness;
        set<int> selected;
        vector<vector<float>> selectedChromosomes;
        vector<float> fitnessSelectedChromosome;
        initializePopulation(pop, lengthOfGene);
        for (int t = 0; t < NUMBEROFITERATIONS; t++)
        {
            Fitness.clear();
            selected.clear();
            calculateFitness(Fitness, Points, pop);
            tournamentSelection(Fitness, selected);
            for (auto a : selected)
                selectedChromosomes.push_back(pop[a]);
            crossOverAndMutation(t, selectedChromosomes);
            calculateFitness(fitnessSelectedChromosome, Points, selectedChromosomes);
            elitistReplacement(Fitness, fitnessSelectedChromosome, selected, pop, selectedChromosomes);
            selectedChromosomes.clear();
            fitnessSelectedChromosome.clear();
            cout << "t " << t << endl;
        }
        MyFile << "Index Of Data Set IS : " << i + 1 << " And ";
        writeOptimalSoultion(MyFile, Fitness, pop);
    }
}
