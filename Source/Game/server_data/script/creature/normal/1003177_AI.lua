-- �������ս������
function c1003177_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 73)

end
aux.RegisterCreatureEvent(1003177, 2, "c1003177_OnEnterCombat")


--�޻ض�Ѩ��ɱ��[1003177]��ˢ��4������
function c1003177_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	--������������
	cre.MonsterSay(MapID, InstanceID, TargetID, 74)
       map.MapCreateCreature(MapID, InstanceID, 1003123, 184, 2540, 396)
	map.MapCreateCreature(MapID, InstanceID, 1003113, 233, 2131, 309)
	map.MapCreateCreature(MapID, InstanceID, 1003173, 233, 2153, 300)
	map.MapCreateCreature(MapID, InstanceID, 1003113, 233, 2140, 291)
	
end
aux.RegisterCreatureEvent(1003177, 4, "c1003177_OnDie")


