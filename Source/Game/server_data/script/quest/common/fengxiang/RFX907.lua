--RFX907
--��216��233��160ˢ����ӡ֮�飨1531005������ӡ֮��1531005��ɱ����ˢ��2����ӡ֮��1531009��ɱ����ӡ֮��1531009��ˢ��3����ӡ֮��1531010
--��������
function c1531005_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
       unit.AddBuff(MapID, InstanceID, RoleID, 2016801, RoleID)
       unit.AddBuff(MapID, InstanceID, RoleID, 2016801, RoleID)
       map.MapCreateCreature(MapID, InstanceID, 1531009, 203, 140, 244)
       map.MapCreateCreature(MapID, InstanceID, 1531009, 226, 140, 244)

end

aux.RegisterCreatureEvent(1531005, 4, "c1531005_OnDie")

--��������
function c1531009_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
       unit.AddBuff(MapID, InstanceID, RoleID, 2016801, RoleID)
       map.MapCreateCreature(MapID, InstanceID, 1531010, 199, 140, 232)
       map.MapCreateCreature(MapID, InstanceID, 1531010, 203, 140, 244)
       map.MapCreateCreature(MapID, InstanceID, 1531010, 226, 140, 244)

end

aux.RegisterCreatureEvent(1531009, 4, "c1531009_OnDie")

--��������
function c1531010_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
       unit.AddBuff(MapID, InstanceID, RoleID, 2016801, RoleID)
end

aux.RegisterCreatureEvent(1531010, 4, "c1531010_OnDie")


--�����ȡʱ����
--function x11907_OnAccept(MapID, InstanceID, questID, ownerID, accepterID)
--	role.RoleGotoNewMap(MapID, InstanceID, ownerID, 1826990375, 171, 5, 32)
--end

--ע��
--aux.RegisterQuestEvent(11907, 0, "x11907_OnAccept")

