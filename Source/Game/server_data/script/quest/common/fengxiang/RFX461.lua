--�ڱ�����11461
--����1�����������ɫɽ����[3020438]ʱ50%������������ˢ�¹���ֱ���Գ[1004106]

--������ɫɽ����������ű�
function c3020438_OnInvest(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local Rate = math.random(1, 100)
	if Rate <= 50 then
		local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
		map.MapCreateCreature(MapID, InstanceID, 1004106, x+2, y, z+2)
	end
end

--ע��
aux.RegisterCreatureEvent(3020438, 6, "c3020438_OnInvest")
