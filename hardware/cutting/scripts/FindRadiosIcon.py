from pyradios import RadioBrowser

rb = RadioBrowser()

for i in rb.search(name="Jazz FM", name_exact=False):
    print(i["favicon"])