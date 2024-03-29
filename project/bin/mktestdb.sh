#!/bin/bash

rm -f db.sqlite3

sqlite3 db.sqlite3 1>/dev/null <<eof
create table newsgroups (id int primary key, 
                         name varchar(255) not null unique);

create table articles (id int primary key, 
                       title varchar(255) not null, 
                       author varchar(255) not null,
                       content text not null, 
                       newsgroup_id int not null,
                       foreign key (newsgroup_id) references newsgroups(id));

insert into newsgroups (id, name) values
    (1, 'comp.lang.c++'), (2, 'comp.lang.c'), (3, 'alt.religion.emacs');

insert into articles (id, title, author, content, newsgroup_id) values
    (1, 'yo dawg', 'dixie normous', 
    'i heard yo like queries so i done put some queries in yo database', 1),
    (2, 'bitches', 'a. guy',  'they dont know bout my dick', 2);
eof
