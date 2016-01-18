/*-- Neues Script --*/

#strict 2

#appendto 1HUD

public func Ammo(int iAmmoCount, int iAmmoLoad, string szName, bool fShow, int dwColorW, int dwColorM)
{
	if(iAmmoCount == -1) {
		SetVisibility(VIS_None, CharsWAmmo[0]);
		SetVisibility(VIS_None, CharsWAmmo[2]);
		CharsWAmmo[1]->Set(-1, dwColorW);
  }
  else {
  	if(GetVisibility() == VIS_Owner) {
  		SetVisibility(VIS_Owner, CharsWAmmo[0]);
			SetVisibility(VIS_Owner, CharsWAmmo[2]);
		}

		var wAmmo = Format("%03d", iAmmoCount);
		var i = 0;
		//dwColorW zur Färbung der ersten Zahl (Momentan geladene Munition)
		if(!dwColorW) dwColorW = ColorEmpty()*(!iAmmoCount);

		for(var char in CharsWAmmo)
		{
		  char->Set(GetChar(wAmmo, i), dwColorW);
		  i++;
		}
	}

	if(iAmmoLoad != -1) {
		var mAmmo = Format("%03d", iAmmoLoad);
		var i = 0;
		for(var char in CharsMaxAmmo)
		{
		  //dwColorM zur Färbung der zweiten Zahl (Maximale Munition)
		  char->Set(GetChar(mAmmo, i), dwColorM);
		  i++;
		}
	}

  if(GetVisibility(CharsClonkAmmo[0]) == VIS_Owner)
  {
    for(var char in CharsClonkAmmo)
      SetVisibility(VIS_None, char);

    SetVisibility(VIS_None, CharEqS);
  }

  if(szName)
    CustomMessage(Format("@%s",szName), this, NO_OWNER, 0, 60, 0, 0, 0, MSG_Left);

  return 1;
}
