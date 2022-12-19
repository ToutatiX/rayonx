from pyradios import RadioBrowser

rb = RadioBrowser()

for i in rb.search(name="Europe 1", name_exact=False):
    print(i['name'], '\t', i['url_resolved'], '\t', i['url'], '\t', i['bitrate'], '\t', i['codec'])
    # print(i)