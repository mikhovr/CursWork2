#encoding "utf-8"
#GRAMMAR_ROOT O
#include "abbr.cxx"


LatCand -> Word<h-reg1, lat> | Word<h-reg1, lat> (Punct) Word<h-reg1, lat>+;
LatCand -> Word<h-reg2, lat> | Word<h-reg2, lat> (Punct) Word<h-reg2, lat>+;

Number -> AnyWord<wff=/[0-9]+/>;
Number -> AnyWord<wfm=/[0-9]+/>;
Number -> AnyWord<wfl=/[0-9]+/>;

AdjAndAdj -> Adj<gnc-agr[1]> (SimConjAnd) Adj<gnc-agr[1]>;

NP -> Noun;
NP -> Noun<gram="gen">;
NP -> Adj<gnc-agr[1]> Noun<gnc-agr[1], rt>;
NP -> AdjAndAdj<gnc-agr[1]> Noun<gnc-agr[1], rt>;
NP -> Noun<rt> Noun<gram="gen">;

NumPunct -> Number | Colon;
WordAndNumPunct -> Word (NumPunct);

GovOrg -> Noun<kwtype="правительство_дескр", rt> ('по') NP<c-agr[1]> SimConjAnd NP<c-agr[1]>*;
GovOrg -> Noun<kwtype="правительство_дескр", rt> ('по') NP  NP<gram="gen">*;

GovOrg -> Noun<kwtype="правительство_сокр"> (Noun<gram="gen">);
GovOrg -> Noun<kwtype="правительство_сокр"> (AbbrCand);
GovOrg -> Noun<kwtype="правительство_гео", rt> (Noun<gram="gen">);
GovOrg -> Noun<kwtype="правительство_гео", rt> (AbbrCand);

AbbrGroup -> AbbrCand<rt> Word<h-reg1, l-quoted> WordAndNumPunct* Word<r-quoted>;
AbbrGroup -> AbbrCand<rt> Word<h-reg1, quoted>;
AbbrGroup -> AbbrCand<rt> Word<h-reg2, quoted>;
AbbrGroup -> AbbrCand<rt> Noun<gram="gen", h-reg1>;

// add cut
MassMedia -> Noun<kwtype="сми_дескр", cut> Word<h-reg1, l-quoted> WordAndNumPunct* Word<r-quoted>;
MassMedia -> Noun<kwtype="сми_дескр", cut> Word<h-reg1, quoted>;
MassMedia -> Noun<kwtype="сми_дескр", cut> Word<h-reg2, quoted>;

CompanyName -> Noun<kwtype="орг_пре"> Word<h-reg1, l-quoted> WordAndNumPunct* Word<r-quoted>;
CompanyName -> Noun<kwtype="орг_пре"> Word<h-reg1, quoted>;
CompanyName -> Noun<kwtype="орг_пре"> Word<h-reg2, quoted>;
CompanyName -> Noun<kwtype="орг_пре"> LatCand;

JustQuotes -> NP<h-reg1, quoted>;
JustQuotes -> Word<h-reg1, l-quoted> Word<~r-quoted>*[gn-agr] Word<r-quoted>;


O -> LatCand | GovOrg | MassMedia | AbbrGroup | CompanyName | JustQuotes;  