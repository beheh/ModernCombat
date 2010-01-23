/*-- Zerstörte Stahlbrücke --*/

#strict 2


func Hit()
{
   //Effekte
   CastParticles("MetalSplinter",4,150,0,0,40,150);
   CastParticles("MetalSplinter",8,230,0,0,40,15,RGB(40,20,20));
   CastParticles("Smoke3",25,15,0,0,50,200);
   Sound("StructuralDamage*.ogg");
}