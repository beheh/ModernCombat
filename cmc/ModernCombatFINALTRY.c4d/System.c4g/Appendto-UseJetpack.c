/*-- UseJetpack --*/

//Fallschirme werden ebenfalls als Jetpacks identifiziert.

#strict 2
#appendto L_JP


public func HasJetpack()
{
  if(this->HasGear(0,JTPK))
    return this->HasGear(0, JTPK);
  else
  if(this->HasGear(0,PPAR))
    return this->HasGear(0, PPAR);
  else
    return false;
}