drop table if exists expense_categories cascade;
drop table if exists income_categories cascade;
drop table if exists bank_accounts cascade;
drop table if exists expenses;
drop table if exists income;

create table expense_categories
(
    id_expense_cat serial primary key unique,
    name           varchar(100) not null
);

create table income_categories
(
    id_income_cat serial primary key unique,
    name          varchar(100) not null
);

create table bank_accounts
(
    id_account serial primary key unique,
    name       varchar(100)  not null,
    amount     int default 0 not null
);

create table expenses
(
    id_expense     serial primary key unique,
    id_expense_cat int                                   not null,
    id_account     int                                   not null,
    amount         double precision default 0            not null,
    date           date             default CURRENT_DATE not null,
    time           time             default CURRENT_TIME not null,
    comment        varchar(200)     default NULL,
    CONSTRAINT id_expense_cat FOREIGN KEY (id_expense_cat) REFERENCES expense_categories (id_expense_cat) ON DELETE CASCADE,
    CONSTRAINT id_account FOREIGN KEY (id_account) REFERENCES bank_accounts (id_account) ON DELETE CASCADE
);

create table income
(
    id_income     serial primary key unique,
    id_income_cat int                                   not null,
    id_account    int                                   not null,
    amount        double precision default 0            not null,
    date          date             default CURRENT_DATE not null,
    time          time             default CURRENT_TIME not null,
    comment       varchar(200)     default NULL,
    CONSTRAINT id_income_cat FOREIGN KEY (id_income_cat) REFERENCES income_categories (id_income_cat) ON DELETE CASCADE,
    CONSTRAINT id_account FOREIGN KEY (id_account) REFERENCES bank_accounts (id_account) ON DELETE CASCADE
);

INSERT INTO bank_accounts(name)
VALUES ('Sberbank'),
       ('Tinkoff'),
       ('VTB');

INSERT INTO expense_categories(name)
VALUES ('Products'),
       ('Transport'),
       ('Cafe'),
       ('Gift'),
       ('Subscription'),
       ('Health');

INSERT INTO income_categories(name)
VALUES ('Salary'),
       ('Cashback'),
       ('Parents'),
       ('Gift'),
       ('interest on the deposit'),
       ('Dividends');

INSERT INTO expenses(id_expense_cat, id_account, amount, date, time)
VALUES (1, 1, 5324, '2022-12-31', '12:40'),
       (2, 2, 98, '2023-01-29', '13:31'),
       (3, 3, 1238, '2023-01-12', '16:01'),
       (4, 1, 7654, '2023-02-21', '22:59'),
       (5, 2, 365, '2023-02-25', '09:32'),
       (6, 3, 6570, '2023-03-07', '20:57');

INSERT INTO income(id_income_cat, id_account, amount, date, time)
VALUES (1, 1, 10000, '2022-12-23', '11:22'),
       (2, 2, 20000, '2023-01-15', '16:31'),
       (3, 3, 30000, '2023-01-01', '04:16'),
       (4, 1, 40000, '2023-02-09', '15:38'),
       (5, 2, 50000, '2023-02-17', '21:17'),
       (6, 3, 60000, '2023-03-06', '12:42');

