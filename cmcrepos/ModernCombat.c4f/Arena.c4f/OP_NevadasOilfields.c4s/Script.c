/* Nevadas Ölfelder */

#strict
#include CSTD

static aFlag;

func ChooserRuleConfig()
{
  return [NOFF,NODR];
}

protected func Initialize() {
  //Hintergrundbewegung
  SetSkyParallax(0,20,10);
  //Musiktitel, welches für dieses Szenario bestimmt ist, zuerst abspielen
  Music("Crisis");
  //Flaggen
  aFlag = [];
  //Szenario einrichten
  CreateFurniture();
  //Ausrüstung plazieren
  CreateEquipment();
  return(1);
  }

protected func CreateFurniture()
{
 
  //Flaggen
  aFlag[0] = CreateObject(OFPL,230,340,NO_OWNER);
    aFlag[0]->Set("$Flag1$",100);
    aFlag[0]->Capture(1);

  aFlag[1] = CreateObject(OFPL,474,230,NO_OWNER);
    aFlag[1]->Set("$Flag2$",100);

  aFlag[2] = CreateObject(OFPL,1469,500,NO_OWNER);
    aFlag[2]->Set("$Flag3$",100);

  aFlag[3] = CreateObject(OFPL,2466,230,NO_OWNER);
    aFlag[3]->Set("$Flag4$",100);

  aFlag[4] = CreateObject(OFPL,2709,340,NO_OWNER);
    aFlag[4]->Set("$Flag5$",100);
    aFlag[4]->Capture(2);

  aFlag[5] = CreateObject(OFPL,1467,790,NO_OWNER);
    aFlag[5]->Set("$Flag6$",100);
   
  CreateObject(PBRL,197,340,-1)->AutoRespawn();
  CreateObject(PBRL,204,340,-1)->AutoRespawn();
  CreateObject(PBRL,612,790,-1)->AutoRespawn();
  CreateObject(PBRL,619,790,-1)->AutoRespawn();
  CreateObject(PBRL,485,230,-1)->AutoRespawn();
  CreateObject(PBRL,492,230,-1)->AutoRespawn();
  CreateObject(PBRL,931,500,-1)->AutoRespawn();
  CreateObject(PBRL,152,500,-1)->AutoRespawn();
  CreateObject(PBRL,1410,790,-1)->AutoRespawn();
  CreateObject(PBRL,1417,790,-1)->AutoRespawn();
  CreateObject(PBRL,1123,750,-1)->AutoRespawn();
  CreateObject(PBRL,1076,460,-1)->AutoRespawn();
  CreateObject(PBRL,152,740,-1)->AutoRespawn();
  CreateObject(PBRL,2731,340,-1)->AutoRespawn();
  CreateObject(PBRL,2724,340,-1)->AutoRespawn();
  CreateObject(PBRL,2787,500,-1)->AutoRespawn();
  CreateObject(PBRL,1999,500,-1)->AutoRespawn();
  CreateObject(PBRL,1850,460,-1)->AutoRespawn();
  CreateObject(PBRL,2791,740,-1)->AutoRespawn();
  CreateObject(PBRL,2299,788,-1)->AutoRespawn();
  CreateObject(PBRL,2306,788,-1)->AutoRespawn();
  CreateObject(PBRL,1517,790,-1)->AutoRespawn();
  CreateObject(PBRL,1524,790,-1)->AutoRespawn();
  CreateObject(GSTA,979,790,-1)->AutoRespawn();
  CreateObject(GSTA,1959,790,-1)->AutoRespawn();
  CreateObject(DRCN,228,480,-1);
  CreateObject(DRCN,2711,480,-1);

  CreateObject(LFTP,95, 514,-1)->SetYTop(0);
  CreateObject(LFTP,2845, 514,-1)->SetYTop(0);

  CreateObject(EXSN, 210, 395,-1);
  CreateObject(EXSN, 2732, 393,-1);

  CreateObject(LCKR, 632, 380,-1);
  CreateObject(LCKR, 666, 380,-1);
  CreateObject(LCKR, 2290, 380,-1);
  CreateObject(LCKR, 2272, 380,-1);

  CreateObject(VEN3, 652, 376,-1)->SetCon(40);
  CreateObject(VEN3, 2285, 377,-1)->SetCon(40);

  CreateObject(H24K, 1125, 629,-1);
  CreateObject(H24K, 1815, 629,-1);

  CreateObject(FIEX, 472, 783,-1);
  CreateObject(FIEX, 1292, 783,-1);
  CreateObject(FIEX, 2180, 783,-1);

  return(1);
}

func CreateEquipment()
{
  var tmp;
  Log("$CreatingEquipment$");
  //Alle Objekte folglich von links oben nach rechts unten

  //Munitionskisten (Kugeln)
  tmp = CreateObject(ABOX, 177, 490, -1);
  tmp->Set(AMOC);
  tmp = CreateObject(ABOX, 2766, 490, -1);
  tmp->Set(AMOC);
}