#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <tuple>
#include <fstream>
#include <sstream>

// 드럼 위치 매핑
enum DrumPosition {
    NONE = 0,
    S = 1,  // 스네어
    FT = 2, // 플로어 탐
    MT = 3, // 미드 탐
    HT = 4, // 하이 탐
    HH = 5, // 하이햇
    R = 6,  // 라이드
    RC = 7, // 라이트 크래쉬
    LC = 8  // 레프트 크래쉬
};

// 특정 조건 검사 함수
bool isDangerous(int rightHand, int currentLeft, const std::vector<int>& nextLeftPositions) {
    switch (rightHand) {
        case S: // 오른손 S
            if (currentLeft == FT || currentLeft == HT || currentLeft == R || currentLeft == MT || currentLeft == RC) {
                return true;
            }
            break;

        case FT: // 오른손 FT
            // 제약 없음
            return false;

        case MT: // 오른손 MT
            for (int nextLeft : nextLeftPositions) {
                if (currentLeft == LC || currentLeft == HH || currentLeft == HT || currentLeft == S) {
                    if (nextLeft == RC || nextLeft == R || nextLeft == FT) {
                        return true;
                    }
                }
            }
            break;

        case HT: // 오른손 HT
            if (currentLeft == FT) {
                return true;
            }
            for (int nextLeft : nextLeftPositions) {
                if (currentLeft == LC || currentLeft == HH || currentLeft == S) {
                    if (nextLeft == MT || nextLeft == RC || nextLeft == R || nextLeft == FT) {
                        return true;
                    }
                }
            }
            break;

        case HH: // 오른손 HH
            for (int nextLeft : nextLeftPositions) {
                if (nextLeft == HT || nextLeft == MT || nextLeft == FT || nextLeft == R || nextLeft == RC) {
                    return true;
                }
            }
            break;

        case R: // 오른손 R
            // 제약 없음
            return false;

        case RC: // 오른손 RC
            for (int nextLeft : nextLeftPositions) {
                if (nextLeft == R) {
                    return true;
                }
            }
            break;

        case LC: // 오른손 LC
            for (int nextLeft : nextLeftPositions) {
                if (nextLeft == RC || nextLeft == R) {
                    return true;
                }
            }
            break;
    }
    return false;
}

// 드럼 악보 데이터에서 위험한 동작 검사
void checkDrumScore(const std::vector<std::tuple<int, double, int, int>>& drumData) {
    for (size_t i = 0; i < drumData.size(); ++i) {
        int measure, nextMeasure = 0;
        double time, nextTime = 0;
        int rightPos, leftPos;

        // 현재 데이터 추출
        std::tie(measure, time, rightPos, leftPos) = drumData[i];

        // 다음, 다다음, 다다다음 왼손 위치 추출
        std::vector<int> nextLeftPositions;
        for (size_t j = 1; j <= 3; ++j) {
            if (i + j < drumData.size()) {
                int tempMeasure, tempRightPos, tempLeftPos;
                double tempTime;
                std::tie(tempMeasure, tempTime, tempRightPos, tempLeftPos) = drumData[i + j];
                nextLeftPositions.push_back(tempLeftPos);
            }
        }

        // 위험 여부 검사
        if (isDangerous(rightPos, leftPos, nextLeftPositions)) {
            std::cout << "위험한 동작: Measure " << measure << ", Time " << time << ", RightHand: " << rightPos << ", LeftHand: " << leftPos << std::endl;
        }
    }
}

// 드럼 악보 데이터를 파일에서 읽기
std::vector<std::tuple<int, double, int, int>> readDrumData(const std::string& filePath) {
    std::vector<std::tuple<int, double, int, int>> drumData;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "파일을 열 수 없습니다: " << filePath << std::endl;
        return drumData;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line.find("bpm") != std::string::npos) {
            continue; // 빈 줄 또는 bpm 정보 무시
        }

        std::istringstream iss(line);
        int measure, rightPos, leftPos;
        double time;
        int dummy1, dummy2, dummy3; // 사용하지 않는 값들

        if (iss >> measure >> time >> rightPos >> leftPos >> dummy1 >> dummy2 >> dummy3) {
            drumData.emplace_back(measure, time, rightPos, leftPos);
        }
    }

    file.close();
    return drumData;
}

// 메인 함수
int main() {
    std::string filePath = "./include/codes/codeMOY_easy0.txt"; // 입력 파일 경로

    // 드럼 악보 데이터 읽기
    std::vector<std::tuple<int, double, int, int>> drumData = readDrumData(filePath);

    if (drumData.empty()) {
        std::cerr << "드럼 데이터를 불러오지 못했습니다." << std::endl;
        return 1;
    }

    // 위험한 동작 검사
    checkDrumScore(drumData);

    return 0;
}