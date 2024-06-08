#!/bin/bash
find -name \*.cpp -o -name \*.h -o -name \*.qml | sort | xargs xgettext --c++ --kde --from-code=UTF-8 \
-ki18n:1 -ki18nc:1c,2 -ki18np:1,2 -ki18ncp:1c,2,3 \
-kki18n:1 -kki18nc:1c,2 -kki18np:1,2 -kki18ncp:1c,2,3 \
-kkli18n:1 -kkli18nc:1c,2 -kkli18np:1,2 -kkli18ncp:1c,2,3 \
-kI18N_NOOP:1 -kI18NC_NOOP:1c,2 \
--copyright-holder="Thomas Duckworth" \
--msgid-bugs-address="tduck973564@gmail.com" \
--package-name="org.filotimo.atychia" \
--package-version="0.1" -o po/atychia.pot
