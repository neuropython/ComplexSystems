#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <unordered_map>
#include <cmath>

using TockensType = std::vector<std::string>;
using TockensUniqueSetType = std::unordered_map<std::string, int>;
using TockensUniqueSetSortedType = std::vector<std::pair<std::string, int>>;
using TockensWithRankType = std::vector<std::tuple<std::string, int, int, double>>;
using TockensWithZipfType = std::vector<std::tuple<std::string, int, int, double, double>>;
using TockensWithZipfZipfFittedType = std::vector<std::tuple<std::string, int, int, double, double, double, double, double>>;

TockensType tokenizeTextFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: could not open file " << filename << std::endl;
        return {};
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string text = buffer.str();

    file.close();

    std::regex wordRegex(R"(\b[a-zA-Z]+\b)");

    std::sregex_iterator wordsBegin = std::sregex_iterator(text.begin(), text.end(), wordRegex);
    std::sregex_iterator wordsEnd = std::sregex_iterator();

    TockensType tokens;
    for (std::sregex_iterator i = wordsBegin; i != wordsEnd; ++i) {
        std::smatch match = *i;
        std::string matchStr = match.str();
        std::transform(matchStr.begin(), matchStr.end(), matchStr.begin(), ::tolower);
        tokens.push_back(matchStr);
    }

    return tokens;
};

TockensUniqueSetType countTokens(const TockensType& tokens) {
    TockensUniqueSetType tokenFrequency;
    for (const std::string& token : tokens) {
        ++tokenFrequency[token];
    }
    return tokenFrequency;
}

TockensUniqueSetSortedType sortSet(const TockensUniqueSetType& tokenFrequency) {
    TockensUniqueSetSortedType tokensSorted(tokenFrequency.begin(), tokenFrequency.end());
    auto comparator = [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
        return a.second > b.second;
    };
    std::sort(tokensSorted.begin(), tokensSorted.end(), comparator);
    return tokensSorted;
};

TockensWithRankType rankTokens (const TockensUniqueSetSortedType& tokenRank, double n) {
    TockensWithRankType tokensWithRank;
    int rank = 0;
    for (const auto& pair : tokenRank) {
            rank++;
        tokensWithRank.push_back(std::make_tuple(pair.first, pair.second, rank, (double)pair.second/n));
    }
    return tokensWithRank;  
};

double CalculateC(const TockensWithRankType& tokensWithRank) {
    double c = 0;
    for (const auto& tuple : tokensWithRank) {
        c += 1.0 / std::get<2>(tuple);
    }
    return c;
};

TockensWithZipfType NormalizedZipfLaw(const TockensWithRankType& tokensWithRank, double c) {
    TockensWithZipfType tokensWithZipf;
    for (const auto& tuple : tokensWithRank) {
        double zipf = 1.0 / (std::get<2>(tuple) * c);
        tokensWithZipf.push_back(std::make_tuple(std::get<0>(tuple), std::get<1>(tuple), std::get<2>(tuple), std::get<3>(tuple), zipf));
    }
    return tokensWithZipf;
};

TockensWithZipfZipfFittedType FittingAB(const TockensWithZipfType& tokensWithZipf, double c) {
    std::vector<std::tuple<double, double, double>> results; 
    TockensWithZipfZipfFittedType tokensWithZipfZipfFitted;
    double currentZipfFitted;
    double bestSSR = std::numeric_limits<double>::max();  
    
    double bestA = 0; 
    double bestB = 0; 
    for (int i = 1; i < 100; i++) {
        double a = i / 10.0;  
        for (int j = 1; j <= 100; j++) {
            double b = j / 10.0; 
            double SSR = 0;
            for (const auto& tuple : tokensWithZipf) {
                int rank = std::get<2>(tuple);  
                double observedFrequency = std::get<3>(tuple);
                double expectedZipf = 1.0 / pow(((rank + b)  * c), a);        
                SSR += pow(observedFrequency - expectedZipf, 2);
            }
            results.push_back(std::make_tuple(a, b, SSR));
            if (SSR < bestSSR) {
                bestSSR = SSR;
                bestA = a;
                bestB = b;
            }
        }
    }
    for (const auto& tuple : tokensWithZipf) {
        int rank = std::get<2>(tuple);
        double observedFrequency = std::get<3>(tuple);
        double zipf = std::get<4>(tuple);
        currentZipfFitted = 1.0 / pow((rank + bestB) *c, bestA);
        tokensWithZipfZipfFitted.push_back(std::make_tuple(std::get<0>(tuple), std::get<1>(tuple), std::get<2>(tuple), std::get<3>(tuple), std::get<4>(tuple), currentZipfFitted, bestA, bestB));
    } 
    return tokensWithZipfZipfFitted;

    std::cout << "Best a: " << bestA << " Best b: " << bestB << " Min SSR: " << bestSSR << std::endl;
}

void writeToCSV(const TockensWithZipfType& tokensWithRank, const std::string& filename) {
    std::ofstream file(filename); 
    if (!file.is_open()) {
        std::cerr << "Error: could not open file " << filename << std::endl;
        return;
    }
    file << "Token,Frequency,Rank,Frequency/N,Zipf" << std::endl;
    for (const auto& tuple : tokensWithRank) {
        file << std::get<0>(tuple) << "," << std::get<1>(tuple) << "," << std::get<2>(tuple) << "," << std::get<3>(tuple) << "," << std::get<4>(tuple) << std::endl;
    }
    file.close();
};

void writeFittedToCSV(const TockensWithZipfZipfFittedType& tokensWithRank, const std::string& filename) {
    std::ofstream file(filename); 
    if (!file.is_open()) {
        std::cerr << "Error: could not open file " << filename << std::endl;
        return;
    }
    file << "Token,Frequency,Rank,Frequency/N,Zipf,FittedZipf,A,B" << std::endl;
    for (const auto& tuple : tokensWithRank) {
        file << std::get<0>(tuple) << "," << std::get<1>(tuple) << "," << std::get<2>(tuple) << "," << std::get<3>(tuple) << "," << std::get<4>(tuple) << "," << std::get<5>(tuple) << "," << std::get<6>(tuple) << "," << std::get<7>(tuple) << std::endl;
    }
    file.close();
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <book_name>" << std::endl;
        return 1;
    }

    std::string bookName = argv[1];
    std::string filePath = "books/" + bookName;
    std::string output = "data/" + bookName + ".csv";
    std::string outputFitted = "data_fitted/" + bookName + "Fitted.csv";

    std::cout << "Reading from: " << filePath << std::endl;
    std::cout << "Output CSV: " << output << std::endl;
    std::cout << "Output Fitted CSV: " << outputFitted << std::endl;

    std::ifstream infile(filePath);
    if (!infile.good()) {
        std::cerr << "Error: File " << filePath << " does not exist." << std::endl;
        return 1;
    }
    infile.close();

    TockensType tokens = tokenizeTextFile(filePath);


    double n = tokens.size();
    TockensUniqueSetType tokenFrequency = countTokens(tokens);
    TockensUniqueSetSortedType TokensSorted = sortSet(tokenFrequency);
    TockensWithRankType tokensWithRank = rankTokens(TokensSorted, n);
    double c = CalculateC(tokensWithRank);
    TockensWithZipfType tokensWithZipf = NormalizedZipfLaw(tokensWithRank, c);
    TockensWithZipfZipfFittedType tokensWithZipfFitted = FittingAB(tokensWithZipf, c);
    writeToCSV(tokensWithZipf, output);
    writeFittedToCSV(tokensWithZipfFitted, outputFitted);

    std::cout << "Processing completed successfully." << std::endl;
    return 0;
}