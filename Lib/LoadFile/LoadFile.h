#pragma once
#include <fstream>
#include <sstream>
#include <vector>

inline std::vector<std::vector<int>>LoadFile(const std::string& csvFilePath) {
	//値を入れる配列
	std::vector<std::vector<int>> intArray;
	//ファイル
	std::ifstream file(csvFilePath);
	//ファイルの数値を入れる変数
	std::string line, value;

	//ファイルから読み取った1行をlineに格納
	while (std::getline(file, line)) {
		std::vector<int> row;
		//1行ごとの文字を入れる
		std::istringstream iss(line);
		//issからカンマ区切りでデータを読み込みvalueに格納
		while (std::getline(iss, value, ',')) {
			// int型に変換
			int intValue = std::stoi(value);
			//行に値を入れる
			row.push_back(intValue);
		}

		//行の値を配列に入れる
		intArray.push_back(row);
	}

	return intArray;
}