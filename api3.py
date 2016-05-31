# -*- coding: utf-8 -*-
#! /usr/bin/python3
import requests
import json
import re
import xml.etree.ElementTree as ET
from collections import OrderedDict
import csv
import HTML # http://www.decalage.info/en/python/html

on_wiki=0
not_wiki=0
has_ib =0

def find_def (wikitext):
    "tries to find infobox template category"
    regexp = re.compile(u'(?<=\{\{)([а-яА-Я0-9a-zA-ZёЁ\s\-+\(\)\:]+)\s?(?=\n\s?\|)') #отделяем инфобокс от плашек
    if regexp.findall(wikitext):
        definition = regexp.findall(wikitext)[0]
    else:
        definition =''
    return definition

def f7(seq):
    "fastest duplication removing algorithm"
    seen = set()
    seen_add = seen.add
    return [x for x in seq if not (x in seen or seen_add(x))]

tree = ET.parse('tomita_output.xml') # xml, отданный Томитой
root = tree.getroot()
entities = root[0][0]
leads = root[0][1]

fileformat = '.txt'
source_name = 'original'

with open(source_name+fileformat, 'r') as source:
    orig_text = source.read() # python 3


keys_list = [
'Unlemmatized',
'Position',
'Length',
'Lemmatized',
'TomitaType',
'OnWiki',
'HasRedirect',
'PageTitle',
'Infobox']

entity_table = {}
entity_table.clear()
entity_table['Unlemmatized'] =  []
entity_table['Position'] =  []
entity_table['Length'] =  []
entity_table['Lemmatized'] =  []
entity_table['TomitaType'] =  []
entity_table['OnWiki'] =  []
entity_table['HasRedirect'] =  []
entity_table['PageTitle'] =  []
entity_table['Infobox'] =  []

entity_table = OrderedDict(sorted(entity_table.items(), key=lambda t: keys_list.index(t[0])))

# формируем API-запрос
request_url = "https://ru.wikipedia.org/w/api.php"
request_params = {}
request_params['action'] = 'query'
request_params['format'] = 'json'
request_params['redirects'] ='true'
request_params['prop'] = 'revisions'
request_params['rvprop'] = 'content'
request_params['rvsection'] = '0'


for entity in entities:
    FactID = int(entity.get('FactID'))
    LeadID = int(entity.get('LeadID'))
    
    NameField = entity[0]
    NameLexem = NameField.get('val')

    Position = int(entity.get('pos'))
    NameLength = int(entity.get('len'))
    
    TypeField = entity[1]
    Type = TypeField.get('val')

    FieldsInfo = entity.get('FieldsInfo')[:-1] #delete ';' 
    text_xml = leads[LeadID].get('text')

    cur_root = ET.fromstring(text_xml) # New sub-XML

    tag = cur_root.findall('.//N[@' + FieldsInfo + ']')[0]
    unlemmatized_entity = re.sub(u'[\"«»]', u'',tag.text)

    lemmatized_entity = tag.get('lemma')

    entity_table['Unlemmatized'].append(unlemmatized_entity)
    entity_table['Position'].append(Position)
    entity_table['Length'].append(NameLength)
    entity_table['Lemmatized'].append(lemmatized_entity)
    entity_table['TomitaType'].append(Type)

for lem in entity_table['Lemmatized']:
    
    request_params['titles'] = lem  #запрос на страницу с именем 'lem'
    response=requests.get(request_url, params=request_params)
    parsed = json.loads(response.text)
    
    if int(list(parsed['query']['pages'].keys())[0])!=-1: #если страница существует (у несуществующей id = -1)
        on_wiki += 1
        entity_table['OnWiki'].append(True)
        
        if 'redirects' in parsed['query'].keys(): #если она редиректится куда-то
            entity_table['HasRedirect'].append(True)
        else:
            entity_table['HasRedirect'].append(False)
            
        elem = list(parsed['query']['pages'].values())[0]
        entity_table['PageTitle'].append(elem['title'])
                
        wikitext = elem['revisions'][0]['*'][:300] #wikitext в третьем элементе
        definition = find_def(wikitext)
        if definition:
            has_ib +=1
            entity_table['Infobox'].append(definition)
        else:
            entity_table['Infobox'].append('No Infobox data')
    else: 
        not_wiki += 1
        entity_table['OnWiki'].append(False)
        entity_table['HasRedirect'].append(False)
        entity_table['PageTitle'].append(False)
        entity_table['Infobox'].append('Non-wiki')

out_table = open("entity_table.csv", "w")
csvwriter = csv.writer(out_table)
csvwriter.writerow(keys_list)
for ctr in range(len(list(entity_table.values())[0])):
    print([item[ctr] for item in list(entity_table.values())])
    csvwriter.writerow([item[ctr] for item in list(entity_table.values())])
out_table.close()

out_table = open("entity_table.csv", "r")
csvreader = csv.reader(out_table, delimiter = ',')
table_data = list(csvreader)

with open('Pretty.html', 'w', encoding = 'ascii', errors='xmlcharrefreplace') as htmlfile:

    htmlfile.write('<html>')
    htmlfile.write('<head>')
    htmlfile.write('<title>Output</title>')
    htmlfile.write('</head>')
    htmlfile.write('<body>')

    redlist = [entity for entity in entity_table['Unlemmatized'] if entity_table['TomitaType'][entity_table['Unlemmatized'].index(entity)]  == 'ПЕРСОНА']
    greenlist = [entity for entity in entity_table['Unlemmatized'] if entity_table['TomitaType'][entity_table['Unlemmatized'].index(entity)]  == 'ЛОКАЦИЯ']
    bluelist = [entity for entity in entity_table['Unlemmatized'] if entity_table['TomitaType'][entity_table['Unlemmatized'].index(entity)]  == 'ОРГАНИЗАЦИЯ']

    colordic = {} 

    for item in f7(redlist):
        colordic[item] = '<font color = \'red\'>'+item+'</font>'

    for item in f7(greenlist):
        colordic[item] = '<font color = \'green\'>'+item+'</font>'

    for item in f7(bluelist):
        colordic[item] = '<font color = \'blue\'>'+item+'</font>'

    for item in colordic:
        orig_text = orig_text.replace(item, colordic[item])

    htmlfile.write(orig_text)
    htmlfile.write('<br>')

    htmltable = HTML.table(table_data)

    htmlfile.write(htmltable)
    htmlfile.write('</body>')
    htmlfile.write('</html>')

print('\nEntities on Wikipedia:', on_wiki)
print('Entities not on Wikipedia:', not_wiki)
print('Entities having infobox data:', has_ib)
print('\nYou can check Pretty.html for pretty markup.')
source.close()
