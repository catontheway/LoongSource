--����ֵ˲��20���ߣ�1000801��

--ʹ��Ч��
function i1000801_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	role.ModRoleAttValue(MapID, InstanceID, TargetID, 77, 20)
end

--ע��
aux.RegisterItemEvent(1000801, 1, "i1000801_QuestUsable")

--����ֵ˲��60���ߣ�1000802��

--ʹ��Ч��
function i1000802_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	role.ModRoleAttValue(MapID, InstanceID, TargetID, 77, 60)
end

--ע��
aux.RegisterItemEvent(1000802, 1, "i1000802_QuestUsable")

--����ֵ˲��120���ߣ�1000803��
--ʹ��Ч��
function i1000803_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	role.ModRoleAttValue(MapID, InstanceID, TargetID, 77, 120)
end
--ע��
aux.RegisterItemEvent(1000803, 1, "i1000803_QuestUsable")


--����ֵͣ��2Сʱ���ߣ�1000804��
--ʹ��Ч��
function i1000804_QuestUsable(MapID, InstanceID, TypeID, TargetID)
    unit.AddBuff(MapID, InstanceID, TargetID, 3312401)
	role.RoleFixSpirit(MapID, InstanceID, TargetID, 36000)
end

--ע��
aux.RegisterItemEvent(1000804, 1, "i1000804_QuestUsable")


--����ֵͣ��4Сʱ���ߣ�1000805��
--ʹ��Ч��
function i1000805_QuestUsable(MapID, InstanceID, TypeID, TargetID)
    unit.AddBuff(MapID, InstanceID, TargetID, 3312402)
	role.RoleFixSpirit(MapID, InstanceID, TargetID, 72000)
end
--ע��
aux.RegisterItemEvent(1000805, 1, "i1000805_QuestUsable")


--����ֵͣ��8Сʱ���ߣ�1000806��
--ʹ��Ч��
function i1000806_QuestUsable(MapID, InstanceID, TypeID, TargetID)
    unit.AddBuff(MapID, InstanceID, TargetID, 3312403)
	role.RoleFixSpirit(MapID, InstanceID, TargetID, 144000)
end
--ע��
aux.RegisterItemEvent(1000806, 1, "i1000806_QuestUsable")

