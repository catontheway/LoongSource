--�з���� 1004433 ����ս����ˢ�� ��¹�Գ���ID��1004425��x= 744 y= 7248 z= 636
function c1004433_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.MapCreateCreature(MapID, InstanceID, 1004425, 744, 7248, 636)
end
aux.RegisterCreatureEvent(1004433, 4, "c1004433_OnDie")
