**** POKRETANJE APLIKACIJE ****


- Unutar poddirektorijuma hfw nalazi se datoteka hfw.sln koju je potrebno buildovati pre pokretanja rešenja. Build se može raditi kroz Visual Studio koristeći prečice Ctrl + Shift + B ili iz menija u VS, a može se raditi i preko CMD-a.
- Build preko CMD: Pozicionirate se u folder gde se nalazi .sln fajl i pokrenete CMD, pa zatim otkucate msbuild hfw.sln
- Nakon builda unutar direktorijuma x64/Debug će se nalaziti .exe fajl koji se mora pokrenuti u administratorskom režimu rada

* Prilikom rada desilo se da nedostaju određeni dependency za neke datoteke te ih Linker nije mogao pronaći, ako Vam se to desi treba da uradite sledeće: desni klik na Project, zatim Properties, Linker->Input i unutar polja Additional Dependencies dodati liniju: 
Ws2_32.lib;Shlwapi.lib;%(AdditionalDependencies)