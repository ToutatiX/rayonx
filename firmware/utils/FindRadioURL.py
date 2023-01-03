from pyradios import RadioBrowser

rb = RadioBrowser()

radio_name = input("Radio name: ")

for i in rb.search(name=radio_name, name_exact=False):
    print(i['name'], '\t', i['url_resolved'], '\t', i['url'], '\t', i['bitrate'], '\t', i['codec'])
    # print(i)