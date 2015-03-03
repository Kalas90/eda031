#!/bin/bash

rm -f db.sqlite3

sqlite3 db.sqlite3 1>/dev/null <<eof
create table newsgroups (id int primary key, name varchar(255));
create table articles (id int primary key, name varchar(255), content text, newsgroup_id int, 
                       foreign key (newsgroup_id) references newsgroups(id));
insert into newsgroups (id, name) values
    (1, 'comp.lang.c++'), (2, 'comp.lang.c'), (3, 'alt.religion.emacs');
insert into articles (id, name, content) values
    (1, 'yo dawg', 'i heard yo like queries so i done put some queries in yo database'),
    (2, 'bitches', 'they dont know bout my dick');
eof
