/*-- Lenseflare --*/
// Sonnenskript includieren
#include SONE

#strict 2

static const CALC_Shadow=false;

local SonneX, SonneY, Screen;

// Initialisierung
protected func Initialize()
{
	// Nur für den Herrn und Meister sichtbar
	SetVisibility (VIS_Owner(), this);
	if(!FindObject(GetID())) 
	// Das erste Lenseflare-Objekt prüft dass jeder Spieler auch sein Umweltobjekt hat und erstellt den Schatten (wenn aktiviert)
	{
		SetOwner(-1,this); // Unparteiisch sein!
		SetCategory(1 | C4D_MouseIgnore,this); // Nicht bewegen
		SetAction("IchWillNurSpielerZaehlen");
		
		if(CALC_Shadow)
		{
  		Log("Berech!= Schatten...");
  		var Shadow_Res=100;
  		var temp=CreateArray();
      temp=CreateArray(LandscapeWidth()/Shadow_Res);
    	for(var x=0;x<LandscapeWidth()+Shadow_Res;x+=Shadow_Res)
    		temp[x/Shadow_Res]=CreateArray(LandscapeHeight()/Shadow_Res);
    	for(x=0;x<LandscapeWidth();x+=Shadow_Res)
    		for(var y=0;y<LandscapeHeight();y+=Shadow_Res)
    		{
    		  if(!PathFree(x,0,x,y))
    		    temp[x/Shadow_Res][y/Shadow_Res]+=50;
    		  if(!PathFree(0,y,x,y))
    		    temp[x/Shadow_Res][y/Shadow_Res]+=50;
    		  if(!PathFree(LandscapeWidth(),y,x,y))
    		    temp[x/Shadow_Res][y/Shadow_Res]+=50;
    		  if(!PathFree(LandscapeWidth()*3/4,0,x,y))
    		    temp[x/Shadow_Res][y/Shadow_Res]+=50;
    		  if(!PathFree(LandscapeWidth()*1/4,0,x,y))
    		    temp[x/Shadow_Res][y/Shadow_Res]+=50;
    		}
        for(x=0;x<LandscapeWidth();x+=Shadow_Res)
    		  for(y=0;y<LandscapeHeight();y+=Shadow_Res)
    		    if(temp[x/Shadow_Res][y/Shadow_Res])
    		      if(temp[x/Shadow_Res][y/Shadow_Res-1])
    		          if(temp[x/Shadow_Res][y/Shadow_Res+1])
                    CreateParticle("Shadow",AbsX(x),AbsY(y),0,0,20*Shadow_Res-temp[x/Shadow_Res][y/Shadow_Res]
                    ,RGBa(255,255,255,255-(temp[x/Shadow_Res][y/Shadow_Res]+temp[x/Shadow_Res+1][y/Shadow_Res+1]+temp[x/Shadow_Res-1][y/Shadow_Res-1])/3));
		}
    return ;
	}
}

// Initialisierung als Haupt-Lenseflare
public func WerdeHauptlenseflareFuerDeinenHerrn()
{ 
	SetCategory(C4D_StaticBack|C4D_IgnoreFoW|C4D_Foreground|C4D_MouseIgnore);
	for(var i=0; i<MIJON(); ++i)
	{
		Local(i) = CreateObject(GetID());
		SetOwner(GetOwner(),Local(i));
		SetCategory(C4D_Object|C4D_IgnoreFoW|C4D_MouseIgnore|C4D_Foreground,Local(i));
		ObjectSetAction(Local(i),Format("Fleck%d",i%14));
	}
	SetAction("SeiBrav");
	Screen=CreateObject(OSCF,0,0,GetOwner());Screen->Set(GetOwner(),RGBa(255,255,200,127),true);
}

// Erstellt einen Lenseflare und weist ihm einen Besitzer zu
private func ErstelleLenseflare(FuerDiesenSpieler)
{
	var Flare = CreateObject(GetID());
	SetOwner(FuerDiesenSpieler,Flare);
	Flare->WerdeHauptlenseflareFuerDeinenHerrn();
}
			

// Von "SeiBrav" aufgerufen
protected func TuWasBraves()
{
	// Hat es seinen Besitzer verloren? :(
	if((!GetPlayerName(GetOwner())) || GetOwner()==-1)
	{
		for(var i=0; i<MIJON(); ++i)
			RemoveObject(Local(i));
		RemoveObject(this); 
		return ;
	}
	// Sonnenposition anpassen
	BerechneSonne();
	SetPosition(SonneX,SonneY);
	// unskalierten Vektor ermitteln
	var VektorX=GibVektorX(GetOwner());
	var VektorY=GibVektorY(GetOwner());
	var VektorDist=GibVektorDist(GetOwner());
	// Die Modulation ist für alle Flares gleich
	var FarbModulation = RGBa(GibFarbMod(),GibFarbMod(),GibFarbMod(),GibAlphaMod(VektorDist));
	// Genauso auch die Abstandsangabe
	var Abstaende = 1000-BoundBy((3000-VektorDist*10)/3,0,1000);  // Promilleangabe 0 = nahe, 1000 = weit weg; 300 Pixel sind dabei "weit weg"
	var AbstaendeAbsolut = (50*Abstaende)/10; // Wieviele 1/100 Pixel zwischen den einzelnen Lenseflares sind
	// Ist unsere liebe Sonne überhaupt sichtbar (also Tag + unverdeckt)
	var SonneSichtbar = IsDay() && (!GBackSemiSolid(0,0)) && PathFree((GetX()+GetX(GetCursor(GetOwner())))/2,0,GetX(GetCursor(GetOwner())),GetY(GetCursor(GetOwner())));
  
  // Tagsüber die Sonne einblenden
	if(IsDay())
	{
		SetVisibility (VIS_Owner());
	}
	else
	{
		SetVisibility (VIS_None());
	}
	// Alle Lenseflares durchgehen
	for(var i=0; i<MIJON(); ++i)
	{
		if(SonneSichtbar)
		{
			// Tags sichtbar
			SetVisibility (VIS_Owner(), Local(i));
			SetClrModulation(FarbModulation,Local(i));
			var LensDist = (i*AbstaendeAbsolut)/100;
			SetPosition(GibLensPosX(LensDist,VektorX,VektorDist), GibLensPosY(LensDist,VektorY,VektorDist),Local(i));
		}
		else
			// Nachts unsichtbar ODER bei verdeckter Sonne
			SetVisibility (VIS_None(), Local(i));
	}
	// Sonnenstich
  var Gelbstich = BoundBy((500-VektorDist)/10,0,100); // Prozentangabe 0 = weit weg, 100 = nahe; 500 Pixel sind dabei "weit weg"
  var light = BoundBy((1500-VektorDist)/15,0,50);
  var targettrans=1000;
  if(GibMittelpunktX()>GetX())
  {
    if(GetDir(GetCursor(GetOwner())))
    {
      targettrans=Abs(Angle(GetX(),GetY(),GibMittelpunktX(),GibMittelpunktY()))*3;
    }
  }
  else
    if(!GetDir(GetCursor(GetOwner())))
    {
      targettrans=Abs(Angle(GetX(),GetY(),GibMittelpunktX(),GibMittelpunktY()))*3;
    }
      
  if(!SonneSichtbar)
    Screen->SetFading(-50);
  else
  {
    var curtrans=Screen->GetTrans();
    if(curtrans/10<targettrans/10)
      Screen->SetFading(10);
    else if(curtrans/10>targettrans/10)
      Screen->SetFading(-10);
    Screen->SetClr(RGBa(light+Gelbstich,light+Gelbstich,light,255-(light+100)));
  }
}

// Für die Farbmodulation (Entfernung + Nacht)
private func GibHelligkeit()
{
	var ZeitObjekt = FindObject(TIME);
	if(!ZeitObjekt)
		return 100;
	return Local(2, ZeitObjekt);
}

private func GibSonnenMod() // Dämmerung 
{
	var Helligkeit = GibHelligkeit();
	// Folgende Farbwerte sind in Promille
	var Rot = 1000;
	var Grun = 1000;
	var Blau = 1000;
	if(Inside(Helligkeit,1,20)) // Dämmerung -> Rotstich
	{
		var RotStich = Cos(Helligkeit*4,500);
		Grun -= RotStich/2;
		Blau -= RotStich;
	}
	// Nun die Farbe berechnen
	return(RGB((255*Rot)/1000,(255*Grun)/1000,(255*Blau)/1000));
}

private func GibFarbMod()
{
	return((255*GibHelligkeit())/100);
}

private func GibAlphaMod(iEntfernung)
{
	var Staerke = BoundBy((2000-iEntfernung)/20,75,100); // Prozentangabe 75 = weit weg, 100 = nahe; 2000 Pixel sind dabei "weit weg"
	return(255-(255*Staerke)/100); 
}

// Genutzte Gammarampe
global func GammaRampe_Lenseflare()
{
	return 2;
}

// Den (unskalierten) Vektor. Er wird skaliert, um die Lenseflare-Positionen zu errechnen
private func GibVektorX(iSpieler)
{
	return GibMittelpunktX(iSpieler) - SonneX;
}

private func GibVektorY(iSpieler)
{
	return GibMittelpunktY(iSpieler) - SonneY;
}

// Skaliere den Vektor mit der gewünschten Entfernung Lenseflare->Sonne
private func GibLensPosX(iEntfernung, VektorX, VektorDist)
{
	// Skalierung in Promille
	var Skalierung = (iEntfernung*1000) / VektorDist;
	var TempX = (VektorX * Skalierung)/1000;
	return TempX + SonneX;
}

private func GibLensPosY(iEntfernung, VektorY, VektorDist)
{
	// Skalierung in Promille
	var Skalierung = (iEntfernung*1000) / VektorDist;
	var TempY = (VektorY * Skalierung)/1000;
	return TempY + SonneY;
}

// Wieviele Lenseflares hätten's denn gerne
private func MIJON()
{
	// etwa alle 50 Pixel einen Lenseflare
	return(LandscapeWidth() / 2 / 50);
}
