

--ʹ����������ָ�30-50����
function i3301006_QuestUsable(MapID, InstanceID, TypeID, TargetID)
    local i = math.random(30,50)
	role.ModRoleAttValue(MapID, InstanceID, TargetID, 25, i)
end

--ע��
aux.RegisterItemEvent(3301006, 1, "i3301006_QuestUsable")

--ʹ����ʿ������ָ�10-30����
function i3301007_QuestUsable(MapID, InstanceID, TypeID, TargetID)
    local i = math.random(10,30)
	role.ModRoleAttValue(MapID, InstanceID, TargetID, 25, i)
end

--ע��
aux.RegisterItemEvent(3301007, 1, "i3301007_QuestUsable")


