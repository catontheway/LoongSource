--��ɱ �ѱ�Թ�� 1005468 ��20%����ˢ�� �������� 3030419�� ��30%����ˢ�� �������� 3030420��
function c1005468_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local a = math.random(1,100)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	if a > 80 then
	map.MapCreateCreature(MapID, InstanceID, 3030419, x, y, z+5)
	elseif a > 50 then
	map.MapCreateCreature(MapID, InstanceID, 3030420, x, y, z+5)
	end
end
aux.RegisterCreatureEvent(1005468, 4, "c1005468_OnDie")
