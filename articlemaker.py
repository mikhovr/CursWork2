from newspaper import Article # http://newspaper.readthedocs.io
url = 'https://lenta.ru/news/2016/05/31/polskiybezgaz/'

art = Article(url, language = 'ru')

art.download()
art.parse()

with open('test.txt', 'w') as original:
    original.write(art.text)