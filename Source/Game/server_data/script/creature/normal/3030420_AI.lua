--���� �������� 3030420�� �ڵ�ǰλ��ˢ�� ������� 1005413��
function c3030420_OnInvest(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	map.MapCreateCreature(MapID, InstanceID, 1005413, x, y, z+5)

end
aux.RegisterCreatureEvent(3030420, 6, "c3030420_OnInvest")
