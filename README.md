## API

HOST: `localhost:8080`

Methods:
1) POST (data in request body):
   1) Счета: `/account`
   2) Расходы: `/expenses`
   3) Доходы: `/income` 
   4) Категории: `/categories/expenses` или `/categories/income`
2) PUT (new data in request body):
   1) Счета: `/accounts` + new data
   2) Расходы: `/expenses` + new data
   3) Доходы: `/income` + new data
   4) Категории: `/categories/expenses` или `/categories/income` + new data
3) GET (query + data in format `?id=...`):
   1) Счета: `/accounts`
   2) Расходы: `/expenses`
   3) Доходы: `/income`
   4) Категории: `/categories/expenses` или `/categories/income`
4) DELETE (query + data in format `?id=...`):
   1) Счета: `/accounts`
   2) Расходы: `/expenses`
   3) Доходы: `/income`
   4) Категории: `/categories/expenses` или `/categories/income`
