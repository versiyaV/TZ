## Запуск проекта
Чтобы запустить проект, необходимо выполнить следующие шаги:
1. Установить необходимые библиотеки
```bash
apt update && apt install -y build-essential cmake libgtest-dev
```
2. Собрать проект
```bash
mkdir build
cd build
cmake ..
make
```

3. Запусть проект
```bash
./matrix_test_rudakov
```


## Ответы к часть 2
1. Fuzzing-тестирование
Можно использовать AFL или написать самим простой fuzzing тест

```clike
TEST(MatrixFuzzTest, RandomInputs) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> size_dist(1, 1000);
    std::uniform_int_distribution<int> val_dist(-1000, 1000);

    for (int i = 0; i < 1000; ++i) {
        size_t rows = size_dist(gen);
        size_t cols = size_dist(gen);
        
        Matrix m(rows, cols);
        std::generate(m.data(), m.data() + rows * cols, [&]() { return val_dist(gen); });

        // Проверка копирования
        Matrix copy = m;
        EXPECT_EQ(memcmp(m.data(), copy.data(), rows * cols * sizeof(int)), 0);

        // Проверка перемещения
        Matrix moved = std::move(m);
        EXPECT_EQ(m.data(), nullptr);
        EXPECT_EQ(moved.rows(), rows);
    }
}
```

2. Статический анализ и динамический (тестирование на устойчивость)
Для проверки подойдет PVS, ASAN - к примеру удалить деструктор в исходном классе и написать тест, который создает объект
```clike
TEST(MatrixASANTest, WithoutDestructor) {
    Matrix m(1, 1); //PVS-warning here
} // ASAN crash here
```

3. Smoke-тестирование
Доработать класс добавив операции сложения/умножения и получение по возможности обратной матрицы
(отлично подойдет для изучения перегрузки операторов)
Написать smoke-тесты на такие операции 

```clike
TEST(MatrixAddTest, WithUserInput) {
    Matrix result(1,1);
    Matrix m(1, 1);
    cin >> m;
    Matrix n(1, 1);
    cin >> n;
    Matrix c = m + n;
    EXPECT_EQ(c, result);
}
```

3. Проверка на потокобезопасность (тестирование на многопоточность)
Отлично подойдет на таком примере рассказать про SDL-практики
Зачем нужны мьютексы и как их использовать
Какие есть подводные камни с операцией swap, почему в контексте такого класса нужна блокировка

4. Итеграционное тестирование
Написать другой класс, который использует класс матрицу - написать примерные тесты, которые проверяют, правильно ли передается матрица и обрабатываются

5. Property-based тестирование
Проверка идемпотентности swap
Тождественность после копирования
```clike
TEST(MatrixPBTest, CopyCreatesEqualMatrix) {
    Matrix a(rows, cols);
    Matrix b = a;
    EXPECT_EQ(a == b);
}
```


