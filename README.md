## Cервис c HTTP API для учета личных финансов

Cервис c HTTP API для учета личных финансов (расходы, доходы, банковские счета).

В файле [`MEGAADDER.sql`](MEGAADDER.sql) содержится конфигурация базы данных.

Параметры для подключения к базе данных задаются в файле [`DatabaseManager`](/Server/include/Server/DatabaseManager.h).

## API

В случае успешной обработки запроса отправляется соответсвующий ответ (приведен в примере к каждому типу запроса).

В случае ошибки при обработке запроса отправляется ответ с описанием ошибки:

```
HTTP/1.1 400 Bad Request
content-length: ...
content-type: text/plain
server: Boost.Beast/345

"Описание ошибки..."
```

---

<details>
   <summary>
      <code>POST</code> <code>/accounts</code> <code>добавление нового счета</code>
   </summary>

Request example

```http request
POST /accounts HTTP/1.1
Host: localhost
Content-Type: application/json
Content-Length: 41

{
  "name": "Sber",
  "amount": "10000"
}
```

Success response example

```
HTTP/1.1 201 Created
content-length: 0
content-type: text/plain
server: Boost.Beast/345
```

</details>

<details>
   <summary>
      <code>POST</code> <code>/expenses</code> <code>добавление новой опервции (расход)</code>
   </summary>

Request example

```http request
POST /expenses HTTP/1.1
Host: localhost
Content-Type: application/json
Content-Length: 131

{
  "id_cat": "3",
  "id_account": "1",
  "amount": "1000",
  "date": "2022-12-12",
  "time": "12:12",
  "comment": "Pyaterochka"
}
```

Success response example
```
HTTP/1.1 201 Created
content-length: 0
content-type: text/plain
server: Boost.Beast/345
```

</details>

<details>
   <summary>
      <code>POST</code> <code>/income</code> <code>добавление новой опервции (доход)</code>
   </summary>

Request example

```http request
POST /income HTTP/1.1
Host: localhost
Content-Type: application/json
Content-Length: 128

{
  "id_cat": "5",
  "id_account": "2",
  "amount": "1000",
  "date": "2022-12-12",
  "time": "12:12",
  "comment": "Cashback"
}
```

Success response example
```
HTTP/1.1 201 Created
content-length: 0
content-type: text/plain
server: Boost.Beast/345
```

</details>

<details>
   <summary>
      <code>POST</code> <code>/categories/expenses</code> <code>добавление новой категории расходов</code>
   </summary>

Request example

```http request
POST /categories/expenses HTTP/1.1
Host: localhost
Content-Type: application/json
Content-Length: 20

{
  "name": "Cafe"
}
```

Success response example
```
HTTP/1.1 201 Created
content-length: 0
content-type: text/plain
server: Boost.Beast/345
```

</details>

<details>
   <summary>
      <code>POST</code> <code>/categories/income</code> <code>добавление новой категории доходов</code>
   </summary>

Request example

```http request
POST /categories/income HTTP/1.1
Host: localhost
Content-Type: application/json
Content-Length: 20

{
  "name": "Cashback"
}
```


Success response example
```
HTTP/1.1 201 Created
content-length: 0
content-type: text/plain
server: Boost.Beast/345
```

</details>

---

<details>
   <summary>
      <code>PUT</code> <code>/accounts</code> <code>изменение данных счета</code>
   </summary>

Request example

```http request
PUT /accounts HTTP/1.1
Host: localhost
Content-Type: application/json
Content-Length: 61

{
  "id_account": "1",
  "name": "VTB",
  "amount": "10000"
}
```

Success response example

```
HTTP/1.1 200 OK
content-length: 0
content-type: text/plain
server: Boost.Beast/345
```

</details>

<details>
   <summary>
      <code>PUT</code> <code>/expenses</code> <code>изменение данных операции (расходы)</code>
   </summary>

Request example

```http request
PUT /expenses HTTP/1.1
Host: localhost
Content-Type: application/json
Content-Length: 152

{
  "id_expense": "2",
  "id_cat": "3",
  "id_account": "1",
  "amount": "1000",
  "date": "2022-12-12",
  "time": "12:12",
  "comment": "Pyaterochka"
}
```

Success response example

```
HTTP/1.1 200 OK
content-length: 0
content-type: text/plain
server: Boost.Beast/345
```

</details>

<details>
   <summary>
      <code>PUT</code> <code>/income</code> <code>изменение данных операции (доходы)</code>
   </summary>

Request example

```http request
PUT /income HTTP/1.1
Host: localhost
Content-Type: application/json
Content-Length: 151

{
  "id_income": "123",
  "id_cat": "12",
  "id_account": "2",
  "amount": "1000",
  "date": "2022-12-12",
  "time": "12:12",
  "comment": "Cashback"
}
```

Success response example

```
HTTP/1.1 200 OK
content-length: 0
content-type: text/plain
server: Boost.Beast/345
```

</details>

<details>
   <summary>
      <code>PUT</code> <code>/categories</code> <code>/expenses</code> <code>изменение данных категории расходов</code>
   </summary>

Request example

```http request
PUT /categories/expenses HTTP/1.1
Host: localhost
Content-Type: application/json
Content-Length: 37

{
  "id_cat": "2",
  "name": "Cafe"
}
```

Success response example

```
HTTP/1.1 200 OK
content-length: 0
content-type: text/plain
server: Boost.Beast/345
```

</details>

<details>
   <summary>
      <code>PUT</code> <code>/categories</code> <code>/income</code> <code>изменение данных категории доходов</code>
   </summary>

Request example

```http request
PUT /categories/income HTTP/1.1
Host: localhost
Content-Type: application/json
Content-Length: 41

{
"id_cat": "2",
"name": "Cashback"
}
```

Success response example

```
HTTP/1.1 200 OK
content-length: 0
content-type: text/plain
server: Boost.Beast/345
```

</details>

---

<details>
   <summary>
      <code>GET</code> <code>/accounts?{id}=some_id</code> <code>информация о счете</code>
   </summary>

Request example

```http request
GET /accounts?id=1 HTTP/1.1
Host: localhost
```

Success response example

```
HTTP/1.1 200 OK
content-length: 135
content-type: application/json
server: Boost.Beast/345

{
    "account": [
        {
            "id_account": "2",
            "name": "Tinkoff",
            "amount": "10000"
        }
    ]
}
```

</details>

<details>
   <summary>
      <code>GET</code> <code>/expenses?{id}=some_id</code> <code>информация об операции (расходы)</code>
   </summary>

Request example

```http request
GET /expenses?id=3 HTTP/1.1
Host: localhost
```

Success response example

```
HTTP/1.1 200 OK
content-length: 257
content-type: application/json
server: Boost.Beast/345

{
    "expenses": [
        {
            "id_expense": "2",
            "id_cat": "2",
            "id_account": "2",
            "amount": "980",
            "date": "2023-01-29",
            "time": "13:31:00",
            "comment": ""
        }
    ]
}
```

</details>

<details>
   <summary>
      <code>GET</code> <code>/income?{id}=some_id</code> <code>информация об операции (доходы)</code>
   </summary>

Request example

```http request
GET /income?id=3 HTTP/1.1
Host: localhost
```

Success response example

```
HTTP/1.1 200 OK
content-length: 257
content-type: application/json
server: Boost.Beast/345

{
    "income": [
        {
            "id_income": "2",
            "id_cat": "2",
            "id_account": "2",
            "amount": "20000",
            "date": "2023-01-15",
            "time": "16:31:00",
            "comment": ""
        }
    ]
}
```

</details>

<details>
   <summary>
      <code>GET</code> <code>/expenses?{begin}=some_date&{end}=some_date</code> <code>информация об операциях за период (расходы)</code>
   </summary>

Request example

```http request
GET /expenses?begin=2022-12-12&end=2023-12-01 HTTP/1.1
Host: localhost
```

Success response example

```
HTTP/1.1 200 OK
content-length: 1004
content-type: application/json
server: Boost.Beast/345

{
    "begin": "2022-12-12",
    "end": "2023-12-01",
    "expenses": [
        {
            "id_expense": "3",
            "id_cat": "3",
            "id_account": "3",
            "amount": "1238",
            "date": "2023-01-12",
            "time": "16:01:00",
            "comment": ""
        },
        {
            "id_expense": "2",
            "id_cat": "2",
            "id_account": "2",
            "amount": "98",
            "date": "2023-01-29",
            "time": "13:31:00",
            "comment": ""
        },
        {
            "id_expense": "5",
            "id_cat": "5",
            "id_account": "2",
            "amount": "365",
            "date": "2023-02-25",
            "time": "09:32:00",
            "comment": ""
        }
    ]
}
```

</details>

<details>
   <summary>
      <code>GET</code> <code>/income?{begin}=some_date&{end}=some_date</code> <code>информация об операциях за период (доходы)</code>
   </summary>

Request example

```http request
GET /income?begin=2022-12-12&end=2023-12-01 HTTP/1.1
Host: localhost
```

Success response example

```
HTTP/1.1 200 OK
content-length: 1005
content-type: application/json
server: Boost.Beast/345

{
    "begin": "2022-12-12",
    "end": "2023-12-01",
    "income": [
        {
            "id_income": "3",
            "id_cat": "3",
            "id_account": "3",
            "amount": "30000",
            "date": "2023-01-01",
            "time": "04:16:00",
            "comment": ""
        },
        {
            "id_income": "2",
            "id_cat": "2",
            "id_account": "2",
            "amount": "20000",
            "date": "2023-01-15",
            "time": "16:31:00",
            "comment": ""
        },
        {
            "id_income": "5",
            "id_cat": "5",
            "id_account": "2",
            "amount": "50000",
            "date": "2023-02-17",
            "time": "21:17:00",
            "comment": ""
        }
    ]
}
```

</details>

<details>
   <summary>
      <code>GET</code> <code>/categories/expenses?{id}=some_id&{begin}=some_date&{end}=some_date</code> <code>информация о всех операциях за период в категории (расходы)</code>
   </summary>

Request example

```http request
GET /categories/expenses?id=3&begin=2022-12-12&end=2023-12-01 HTTP/1.1
Host: localhost
```

Success response example

```
HTTP/1.1 200 OK
content-length: 330
content-type: application/json
server: Boost.Beast/345

{
    "id_cat": "3",
    "begin": "2022-12-12",
    "end": "2023-12-01",
    "expenses": [
        {
            "id_expense": "3",
            "id_cat": "3",
            "id_account": "3",
            "amount": "1238",
            "date": "2023-01-12",
            "time": "16:01:00",
            "comment": ""
        },
        {
            "id_expense": "2",
            "id_cat": "3",
            "id_account": "2",
            "amount": "98",
            "date": "2023-01-29",
            "time": "13:31:00",
            "comment": ""
        },
        {
            "id_expense": "5",
            "id_cat": "3",
            "id_account": "2",
            "amount": "9999",
            "date": "2023-02-25",
            "time": "09:32:00",
            "comment": ""
        }
    ]
}
```

</details>

<details>
   <summary>
      <code>GET</code> <code>/categories/income?{id}=some_id&{begin}=some_date&{end}=some_date</code> <code>информация о всех операциях за период в категории (доходы)</code>
   </summary>

Request example

```http request
GET /categories/income?id=2&begin=2022-12-12&end=2023-12-01 HTTP/1.1
Host: localhost
```

Success response example

```
HTTP/1.1 200 OK
content-length: 328
content-type: application/json
server: Boost.Beast/345

{
    "id_cat": "2",
    "begin": "2022-12-12",
    "end": "2023-12-01",
    "income": [
        {
            "id_income": "2",
            "id_cat": "2",
            "id_account": "2",
            "amount": "20000",
            "date": "2023-01-15",
            "time": "16:31:00",
            "comment": ""
        }
    ]
}
```

</details>

---

<details>
   <summary>
      <code>DELETE</code> <code>/accounts?{id}=some_id</code> <code>удаляет счет и все связанные с ним расходы и доходы</code>
   </summary>

Request example

```http request
DELETE /accounts?id=3 HTTP/1.1
Host: localhost
```

Success response example

```
HTTP/1.1 200 OK
content-length: 0
content-type: text/plain
server: Boost.Beast/345
```

</details>

<details>
   <summary>
      <code>DELETE</code> <code>/expenses?{id}=some_id</code> <code>удаляет операцию (расходы)</code>
   </summary>

Request example

```http request
DELETE /expenses?id=5 HTTP/1.1
Host: localhost
```

Success response example

```
HTTP/1.1 200 OK
content-length: 0
content-type: text/plain
server: Boost.Beast/345
```

</details>

<details>
   <summary>
      <code>DELETE</code> <code>/income?{id}=some_id</code> <code>удаляет операцию (доходы)</code>
   </summary>

Request example

```http request
DELETE /income?id=5 HTTP/1.1
Host: localhost
```

Success response example

```
HTTP/1.1 200 OK
content-length: 0
content-type: text/plain
server: Boost.Beast/345
```

</details>

<details>
   <summary>
      <code>DELETE</code> <code>/categories/expenses?{id}=some_id</code> <code>удаляет категорию доходов (связанные с ней операции переходят в категорию "Other")</code>
   </summary>

Request example

```http request
DELETE /categories/expenses?id=5 HTTP/1.1
Host: localhost
```

Success response example

```
HTTP/1.1 200 OK
content-length: 0
content-type: text/plain
server: Boost.Beast/345
```

</details>

<details>
   <summary>
      <code>DELETE</code> <code>/categories/income?{id}=some_id</code> <code>удаляет категорию расходов (связанные с ней операции переходят в категорию "Other")</code>
   </summary>

Request example

```http request
DELETE /categories/income?id=5 HTTP/1.1
Host: localhost
```

Success response example

```
HTTP/1.1 200 OK
content-length: 0
content-type: text/plain
server: Boost.Beast/345
```

</details>
