#encoding "utf-8"
#GRAMMAR_ROOT P

PropName -> Noun<h-reg1, gram="persn">;
FamName -> Noun<h-reg1, gram="famn">;
PatName -> Noun<h-reg1, gram="patrn">;

Candidate -> Word<h-reg1, ~h-reg2, c-agr[1], kwset=~[LOC, ORG, ORG_ABBR], ~l-quoted> Word<h-reg1, ~h-reg2, c-agr[1]>*;
Candidate -> UnknownPOS<h-reg1, ~quoted>;

NotNum -> Candidate<wff=/[^0-9]+/>;
NotNum -> Candidate<wfm=/[^0-9]+/>;
NotNum -> Candidate<wfl=/[^0-9]+/>;
NotNum1 -> NotNum<gram=~"NUM">;
NotNum2 -> NotNum1<gram=~"ANUM">;
NotPrep -> NotNum2<gram=~"PR">;
NotConj -> NotPrep<gram=~"CONJ">;
NotVerb -> NotConj<gram=~"V">;
NotProp -> NotVerb<gram=~"SPRO">;
NotAdv -> NotProp<gram=~"ADV">;
NotPart -> NotAdv<gram=~"PART">;
NotAdj -> NotPart<gram=~"A">;
NotNoun -> NotAdj;//    <gram="pl">;
NotNeuter -> NotNoun<gram=~"n">; // neuter gender
NotLat -> NotNeuter<wfm=/[^a-zA-Z]+.?/>;

MbName -> NotLat<GU=[S,anim], fw>;
MbName -> NotLat<~fw>;


P -> PropName (PatName) FamName | FamName | MbName;