--������ ̰���� 3060026 ��30%����ˢ�� ������ɷ��ħ�� 1008081 ���꣺x 2600 y 25797 z 447
function c3060026_OnInvest(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local a = math.random(1,100)
	if a > 70 then
	map.MapCreateCreature(MapID, InstanceID, 1008081, 2600, 25797, 447)
	end
end
aux.RegisterCreatureEvent(3060026, 6, "c3060026_OnInvest")
