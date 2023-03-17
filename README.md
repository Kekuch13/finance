## API

HOST: `localhost:8080`

Methods:

1) POST (data in request body):
    1) Счета: `/accounts` + json
    2) Расходы: `/expenses` + json
    3) Доходы: `/income` + json
    4) Категории: `/categories/expenses` или `/categories/income` + json
2) PUT (new data in request body):
    1) Счета: `/accounts` + json
    2) Расходы: `/expenses` + ......
    3) Доходы: `/income` + ......
    4) Категории: `/categories/expenses` или `/categories/income` + json
3) GET (+ query in format `?id=...`):
    1) Счета: `/accounts`
    2) Расходы: `/expenses`
    3) Доходы: `/income`
    4) Категории: `/categories/expenses` или `/categories/income`
4) DELETE (+ query in format `?id=...`):
    1) Счета: `/accounts`
    2) Расходы: `/expenses`
    3) Доходы: `/income`
    4) Категории: `/categories/expenses` или `/categories/income`
