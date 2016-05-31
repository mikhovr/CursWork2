#encoding "utf-8"
#GRAMMAR_ROOT AbbrCand

WordOk -> Word<kwtype=~"bad_org">;
AbbrCand -> WordOk<gram="abbr"> WordOk<gram="abbr">*;
AbbrCand -> WordOk<wfm=/[A-Я-]{2,10}/> WordOk<wfm=/[A-Я-]{2,10}/>*;
