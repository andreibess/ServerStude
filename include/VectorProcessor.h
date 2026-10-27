/**
 * @file VectorProcessor.h
 * @brief Вычисления над векторами
 */
#pragma once
#include <vector>

/**
 * @class VectorProcessor
 * @brief Содержит функцию для вычисления суммы квадратов.
 */
class VectorProcessor {
public:
    /**
     * @brief Вычисляет сумму квадратов элементов вектора.
     * @param vec Вектор вещественных чисел.
     * @return Сумма квадратов.
     */
    static double sumOfSquares(const std::vector<double>& vec);
};
