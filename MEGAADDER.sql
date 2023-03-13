drop table if exists expense_categories cascade;
drop table if exists income_categories cascade;
drop table if exists bank_accounts cascade;
drop table if exists expenses;
drop table if exists income;

create table expense_categories
(
    id_expense_cat   serial primary key unique,
    name_expense_cat varchar(100) not null
);

create table income_categories
(
    id_income_cat   serial primary key unique,
    name_income_cat varchar(100) not null
);

create table bank_accounts
(
    id_account     serial primary key unique,
    name_account   varchar(100)  not null,
    account_amount int default 0 not null
);

create table expenses
(
    id_expense     serial primary key unique,
    id_expense_cat int                                   not null,
    id_account     int                                   not null,
    amount         double precision default 0            not null,
    date           date             default CURRENT_DATE not null,
    time           time             default CURRENT_TIME not null,
    comment        varchar(200),
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
    comment       varchar(200),
    CONSTRAINT id_income_cat FOREIGN KEY (id_income_cat) REFERENCES income_categories (id_income_cat) ON DELETE CASCADE,
    CONSTRAINT id_account FOREIGN KEY (id_account) REFERENCES bank_accounts (id_account) ON DELETE CASCADE
);

