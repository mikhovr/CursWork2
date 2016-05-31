#encoding "utf-8"    // сообщаем парсеру о том, в какой кодировке написана грамматика
#GRAMMAR_ROOT S      // указываем корневой нетерминал грамматики


Person -> Word<kwtype=PER>+;
Location -> Word<kwtype=LOC>+;
Organization -> Word<kwtype=ORG>+;
Abbr -> Word<kwtype=ORG_ABBR>+;

S -> Person interp(NamedEntity.Name; NamedEntity.Type="персона");
S -> Location interp(NamedEntity.Name; NamedEntity.Type="локация");
S -> Organization interp(NamedEntity.Name; NamedEntity.Type="организация");
S -> Abbr interp(NamedEntity.Name::not_norm; NamedEntity.Type="организация");