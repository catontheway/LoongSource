--------------
--1361113	������仨��
--1361114	������仨��
--1361115	��������
--1361116	���������
--1361117	����úϻ���
--1361118	���������
--1361119	ϲ����Ե����
--1361120	�䰮õ�廨��
--1361121	ϲ�컨ƿ
--1361122	ϲ�컨ƿ
--------------
--4900428	����
--4900429	����
--4900430	����
--4900431	�����
--4900432	һ����
--4900433	һ����
--4900434	һ����
--4900435	õ�廨��
--4900436	��ƿ
--4900437	��ƿ


function I1361113_QuestUsable(MapID, InstanceID, TypeID, TargetID)

	local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
	local a = map.MapCreateCreature(MapID, InstanceID, 4900428, x+2, y, z+2)
end

aux.RegisterItemEvent(1361113, 1, "I1361113_QuestUsable")


function I1361114_QuestUsable(MapID, InstanceID, TypeID, TargetID)

	local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
	local a = map.MapCreateCreature(MapID, InstanceID, 4900429, x+2, y, z+2)
end

aux.RegisterItemEvent(1361114, 1, "I1361114_QuestUsable")

function I1361115_QuestUsable(MapID, InstanceID, TypeID, TargetID)

	local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
	local a = map.MapCreateCreature(MapID, InstanceID, 4900430, x+2, y, z+2)
end

aux.RegisterItemEvent(1361115, 1, "I1361115_QuestUsable")

function I1361116_QuestUsable(MapID, InstanceID, TypeID, TargetID)

	local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
	local a = map.MapCreateCreature(MapID, InstanceID, 4900431, x+2, y, z+2)
end

aux.RegisterItemEvent(1361116, 1, "I1361116_QuestUsable")

function I1361117_QuestUsable(MapID, InstanceID, TypeID, TargetID)

	local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
	local a = map.MapCreateCreature(MapID, InstanceID, 4900432, x+2, y, z+2)
end

aux.RegisterItemEvent(1361117, 1, "I1361117_QuestUsable")

function I1361118_QuestUsable(MapID, InstanceID, TypeID, TargetID)

	local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
	local a = map.MapCreateCreature(MapID, InstanceID, 4900433, x+2, y, z+2)
end

aux.RegisterItemEvent(1361118, 1, "I1361118_QuestUsable")

function I1361119_QuestUsable(MapID, InstanceID, TypeID, TargetID)

	local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
	local a = map.MapCreateCreature(MapID, InstanceID, 4900434, x+2, y, z+2)
end

aux.RegisterItemEvent(1361119, 1, "I1361119_QuestUsable")

function I1361120_QuestUsable(MapID, InstanceID, TypeID, TargetID)

	local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
	local a = map.MapCreateCreature(MapID, InstanceID, 4900435, x+2, y, z+2)
end

aux.RegisterItemEvent(1361120, 1, "I1361120_QuestUsable")

function I1361121_QuestUsable(MapID, InstanceID, TypeID, TargetID)

	local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
	local a = map.MapCreateCreature(MapID, InstanceID, 4900436, x+2, y, z+2)
end

aux.RegisterItemEvent(1361121, 1, "I1361121_QuestUsable")

function I1361122_QuestUsable(MapID, InstanceID, TypeID, TargetID)

	local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
	local a = map.MapCreateCreature(MapID, InstanceID, 4900437, x+2, y, z+2)
end

aux.RegisterItemEvent(1361122, 1, "I1361122_QuestUsable")




