#encoding "utf-8"
#GRAMMAR_ROOT L


NotNum -> AnyWord<wff=/[^0-9]+/>;
NotNum -> AnyWord<wfm=/[^0-9]+/>;
NotNum -> AnyWord<wfl=/[^0-9]+/>;

FirstCand -> NotNum<kwtype=~"bad_loc">;

State -> Word<kwtype='штат'>;
        
GeoCand -> Adj<kwtype='гео_напр', h-reg1> FirstCand<h-reg1, rt>;

MaybeCand -> Adj<h-reg1, gnc-agr[1]> Noun<h-reg1, gnc-agr[1]>;

GeoGram -> (Adj<kwtype='гео_напр'>) FirstCand<gram="geo", rt>;
GeoConst -> GeoGram<geo-agr[1]> GeoGram<geo-agr[1]>*;

NameFam -> (Word<h-reg1, gram="persn", gnc-agr[1]>) Word<h-reg1, gram="famn", gnc-agr[1]>;


PreAddr -> Adj<h-reg1> Adj*;
PreAddr -> Noun<gram="n", h-reg1>;

PostAddr -> NameFam<gram="род">;
PostAddr -> (Adj<gnc-agr[1], h-reg1>) Word<gnc-agr[1], h-reg1, rt> (Word<gram="род", h-reg1>);

AddressDescr -> Word<kwtype='адрес'>;

Address -> PreAddr<nc-agr[1]> AddressDescr<rt, nc-agr[1]> (PostAddr);
Address -> (PreAddr<nc-agr[1]>) AddressDescr<rt, nc-agr[1]> PostAddr;


L -> GeoGram | GeoConst | State | Address | MaybeCand;