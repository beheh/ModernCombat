/*-- Gebäude --*/

//Appendiert an alle Gebäude mit Fundament

#strict 2

/*
#appendto BAS7
#appendto HUT1

protected func Damage(int iChange, int iByPlayer)
{
  if( !Hostile(iByPlayer,GetOwner()) ) return; 
  var Database = FindObject(AR_A);
    
  //Wtf, GetGefCoreVal("Name") == "Basement" ging nicht...
  if( GetID()==BAS1 || GetID()==BAS2 || GetID()==BAS3 || GetID()==BAS4 || GetID()==BAS5 || 
      GetID()==BAS6 || GetID()==BAS7 || GetID()==BAS8 || GetID()==BAS9 || GetID()==BSC1 ||
      GetID()==BSC2 || GetID()==BSC3 || GetID()==B35B ) return;
  if( GetOCF() & OCF_OnFire) return;
  Database -> SetPlayerStats("Bonuspoints", iByPlayer, DestructionPoints(iChange));
  Message("{{%i}}->{{%i}} <c 00ff00>+%d</c>", this(), GetID(), FLAM, DestructionPoints(iChange));
}
*/
