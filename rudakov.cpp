#include <algorithm>
#include <chrono>
#include <cstring>
#include <random>
#include <vector>
#include <utility>

#include <gtest/gtest.h>

class Matrix {
public:
    Matrix(size_t rows, size_t cols) : rows_(rows), cols_(cols), data_(new int[rows * cols]{}) {}

    ~Matrix() { delete[] data_; }

    Matrix(const Matrix &rhd) : rows_(rhd.rows_), cols_(rhd.cols_), data_(new int[rhd.rows_ * rhd.cols_]) {
        std::memcpy(data_, rhd.data_, rows_ * cols_ * sizeof(int));
    }

    Matrix &operator=(const Matrix &rhd) {
        if (this != &rhd) {
            delete[] data_;

            rows_ = rhd.rows_;
            cols_ = rhd.cols_;

            data_ = new int[rhd.rows_ * rhd.cols_];
            std::memcpy(data_, rhd.data_, rows_ * cols_ * sizeof(int));
        }
        return *this;
    }

    Matrix(Matrix&& rhd) noexcept 
        : rows_(rhd.rows_), cols_(rhd.cols_), data_(std::exchange(rhd.data_, nullptr)) {
        rhd.rows_ = 0;
        rhd.cols_ = 0;
    }

    Matrix& operator=(Matrix&& rhd) noexcept {
        if (this != &rhd) {
            delete[] data_;
            rows_ = rhd.rows_;
            cols_ = rhd.cols_;
            data_ = std::exchange(rhd.data_, nullptr);
            rhd.rows_ = 0;
            rhd.cols_ = 0;
        }
        return *this;
    }

    void swap(Matrix& other) noexcept {
        std::swap(rows_, other.rows_);
        std::swap(cols_, other.cols_);
        std::swap(data_, other.data_);
    }

    friend void swap(Matrix& lhs, Matrix& rhs) noexcept {
        lhs.swap(rhs);
    }

    size_t rows() const { return rows_; }
    size_t cols() const { return cols_; }
    int* data() const { return data_; }

private:
    size_t rows_;
    size_t cols_;
    int *data_;
};



TEST(MatrixTest, Constructor) {
    Matrix m(2, 3);
    EXPECT_EQ(m.rows(), 2);
    EXPECT_EQ(m.cols(), 3);
    EXPECT_NE(m.data(), nullptr);
}

TEST(MatrixTest, CopyConstructor) {
    Matrix m1(2, 3);
    m1.data()[0] = 42;
    
    Matrix m2(m1);
    EXPECT_EQ(m2.rows(), 2);
    EXPECT_EQ(m2.cols(), 3);
    EXPECT_NE(m2.data(), nullptr);
    EXPECT_NE(m1.data(), m2.data());
    EXPECT_EQ(m2.data()[0], 42);
}

TEST(MatrixTest, CopyAssignment) {
    Matrix m1(2, 3);
    m1.data()[0] = 42;
    
    Matrix m2(1, 1);
    m2 = m1;
    
    EXPECT_EQ(m2.rows(), 2);
    EXPECT_EQ(m2.cols(), 3);
    EXPECT_NE(m2.data(), nullptr);
    EXPECT_NE(m1.data(), m2.data());
    EXPECT_EQ(m2.data()[0], 42);
}

TEST(MatrixTest, MoveConstructor) {
    Matrix m1(2, 3);
    int* original_data = m1.data();
    m1.data()[0] = 42;
    
    Matrix m2(std::move(m1));
    
    EXPECT_EQ(m2.rows(), 2);
    EXPECT_EQ(m2.cols(), 3);
    EXPECT_EQ(m2.data(), original_data);
    EXPECT_EQ(m2.data()[0], 42);
    
    EXPECT_EQ(m1.rows(), 0);
    EXPECT_EQ(m1.cols(), 0);
    EXPECT_EQ(m1.data(), nullptr);
}

TEST(MatrixTest, MoveAssignment) {
    Matrix m1(2, 3);
    int* original_data = m1.data();
    m1.data()[0] = 42;
    
    Matrix m2(1, 1);
    m2 = std::move(m1);
    
    EXPECT_EQ(m2.rows(), 2);
    EXPECT_EQ(m2.cols(), 3);
    EXPECT_EQ(m2.data(), original_data);
    EXPECT_EQ(m2.data()[0], 42);
    
    EXPECT_EQ(m1.rows(), 0);
    EXPECT_EQ(m1.cols(), 0);
    EXPECT_EQ(m1.data(), nullptr);
}

TEST(MatrixTest, MemberSwap) {
    Matrix m1(2, 3);
    m1.data()[0] = 42;
    int* m1_data = m1.data();
    
    Matrix m2(4, 5);
    m2.data()[0] = 24;
    int* m2_data = m2.data();
    
    m1.swap(m2);
    
    EXPECT_EQ(m1.rows(), 4);
    EXPECT_EQ(m1.cols(), 5);
    EXPECT_EQ(m1.data(), m2_data);
    EXPECT_EQ(m1.data()[0], 24);
    
    EXPECT_EQ(m2.rows(), 2);
    EXPECT_EQ(m2.cols(), 3);
    EXPECT_EQ(m2.data(), m1_data);
    EXPECT_EQ(m2.data()[0], 42);
}

TEST(MatrixTest, FriendSwap) {
    Matrix m1(2, 3);
    m1.data()[0] = 42;
    int* m1_data = m1.data();
    
    Matrix m2(4, 5);
    m2.data()[0] = 24;
    int* m2_data = m2.data();
    
    swap(m1, m2);
    
    EXPECT_EQ(m1.rows(), 4);
    EXPECT_EQ(m1.cols(), 5);
    EXPECT_EQ(m1.data(), m2_data);
    EXPECT_EQ(m1.data()[0], 24);
    
    EXPECT_EQ(m2.rows(), 2);
    EXPECT_EQ(m2.cols(), 3);
    EXPECT_EQ(m2.data(), m1_data);
    EXPECT_EQ(m2.data()[0], 42);
}

TEST(MatrixTest, SelfAssignment) {
    Matrix m1(2, 3);
    m1.data()[0] = 42;
    int* original_data = m1.data();
    
    m1 = m1;
    
    EXPECT_EQ(m1.rows(), 2);
    EXPECT_EQ(m1.cols(), 3);
    EXPECT_EQ(m1.data(), original_data);
    EXPECT_EQ(m1.data()[0], 42);
    
    m1 = std::move(m1);
    
    EXPECT_EQ(m1.rows(), 2);
    EXPECT_EQ(m1.cols(), 3);
    EXPECT_EQ(m1.data(), original_data);
    EXPECT_EQ(m1.data()[0], 42);
}

TEST(MatrixExceptionTest, NoexceptMove) {
    EXPECT_TRUE(std::is_nothrow_move_constructible<Matrix>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<Matrix>::value);
    EXPECT_TRUE(noexcept(std::declval<Matrix>().swap(std::declval<Matrix&>())));
}


TEST(MatrixPerformanceTest, MoveVsCopy) {
    const size_t large_size = 10000;
    Matrix large_matrix(large_size, large_size);
    
    auto start_copy = std::chrono::high_resolution_clock::now();
    Matrix copy = large_matrix;
    auto end_copy = std::chrono::high_resolution_clock::now();
    
    auto start_move = std::chrono::high_resolution_clock::now();
    Matrix moved = std::move(large_matrix);
    auto end_move = std::chrono::high_resolution_clock::now();
    
    auto copy_time = end_copy - start_copy;
    auto move_time = end_move - start_move;
    
    EXPECT_LT(move_time, copy_time / 10);
}

TEST(MatrixPerformanceTest, ComprehensiveBenchmark) {
    constexpr size_t small_size = 100;
    constexpr size_t medium_size = 1000;
    constexpr size_t large_size = 10000;
    constexpr int warmup_runs = 3;
    constexpr int measurement_runs = 10;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 100);
    
    auto benchmark_operation = [&](auto&& op, const std::string& name, size_t size) {
        for (int i = 0; i < warmup_runs; ++i) {
            op();
        }

        std::vector<double> times;
        for (int i = 0; i < measurement_runs; ++i) {
            auto start = std::chrono::high_resolution_clock::now();
            op();
            auto end = std::chrono::high_resolution_clock::now();
            times.push_back(std::chrono::duration<double, std::milli>(end - start).count());
        }

        auto [min, max] = std::minmax_element(times.begin(), times.end());
        double avg = std::accumulate(times.begin(), times.end(), 0.0) / times.size();
        
        std::cout << name << " (" << size << "x" << size << "): "
                  << "avg=" << avg << "ms, min=" << *min << "ms, max=" << *max << "ms\n";
    };

    for (size_t size : {small_size, medium_size, large_size}) {
        Matrix m1(size, size);
        Matrix m2(size, size);

        std::generate(m1.data(), m1.data() + size*size, [&](){ return dist(gen); });
        std::generate(m2.data(), m2.data() + size*size, [&](){ return dist(gen); });

        benchmark_operation([&](){ Matrix copy = m1; }, "Copy construction", size);
        benchmark_operation([&](){ Matrix moved = std::move(m1); }, "Move construction", size);
        benchmark_operation([&](){ m2 = m1; }, "Copy assignment", size);
        benchmark_operation([&](){ m2 = std::move(m1); }, "Move assignment", size);
        benchmark_operation([&](){ m1.swap(m2); }, "Member swap", size);
        benchmark_operation([&](){ swap(m1, m2); }, "Friend swap", size);

        if (m1.data() == nullptr) {
            m1 = Matrix(size, size);
            std::generate(m1.data(), m1.data() + size*size, [&](){ return dist(gen); });
        }
    }
}

TEST(MatrixADLTest, ADLSwap) {
    Matrix m1(2, 3);
    Matrix m2(4, 5);
    
    swap(m1, m2);
    
    EXPECT_EQ(m1.rows(), 4);
    EXPECT_EQ(m2.rows(), 2);
}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}