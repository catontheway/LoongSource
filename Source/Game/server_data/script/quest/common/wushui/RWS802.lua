--������ʯ��13802
--����1�������ĸ�����[3040601, 3040602, 3040603, 3040604]��ֱ�ˢ�³���������[1006603]��ˮ���[1006604]�������[1006605]�������[1006606]
--���꣺
--[m05: 1664, 17345, 1565]
--[m05: 2441, 16878, 2761]
--[m05: 1999, 24790, 1408]
--[m05: 2586, 25637, 1695]

--����1�����麯��
function c3040601_OnInvest(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.MapCreateCreature(MapID, InstanceID, 1006603, 1664, 17345, 1565)    --�����
end

--����2�����麯��
function c3040602_OnInvest(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.MapCreateCreature(MapID, InstanceID, 1006604, 2441, 16878, 2761)    --ˮ���
end

--����3�����麯��
function c3040603_OnInvest(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.MapCreateCreature(MapID, InstanceID, 1006605, 1999, 24790, 1408)    --�����
end

--����4�����麯��
function c3040604_OnInvest(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.MapCreateCreature(MapID, InstanceID, 1006606, 2586, 25637, 1695)    --�����
end

--ע��
aux.RegisterCreatureEvent(3040601, 6, "c3040601_OnInvest")
aux.RegisterCreatureEvent(3040602, 6, "c3040602_OnInvest")
aux.RegisterCreatureEvent(3040603, 6, "c3040603_OnInvest")
aux.RegisterCreatureEvent(3040604, 6, "c3040604_OnInvest")
