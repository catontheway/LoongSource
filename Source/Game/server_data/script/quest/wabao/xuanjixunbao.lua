--�Ƿ����ʹ���ƾ�ͼ
function I1350002_CanUseItem(MapID, InstanceID, TypeID, TargetID)
    if role.IsRoleHaveQuest(MapID, InstanceID, TargetID, 20034) or role.IsRoleHaveQuest(MapID, InstanceID, TargetID, 20036) then
        return 45, false
    elseif role.IsRoleHaveQuest(MapID, InstanceID, TargetID, 20035) then
    	return 41, false
    end

	--�жϱ����Ƿ��еط�����ᱦͼ
	local BagFreeSize = role.GetBagFreeSize(TargetID)
	if(BagFreeSize <= 0) then
		return 40, false
	end

	return 0, false
end

--�Ƿ����ʹ����ᱦͼ
function I1350005_CanUseItem(MapID, InstanceID, TypeID, TargetID)
    --�������ѵ�����ָ�������
	local bool = WaBao_EnterNearArea(MapID, InstanceID, TargetID)
	--�����Ҳ���ָ�����긽������ָ�����귢���ͻ���
	if bool == false then
		--��ʾ��� ��Ѱ���ص㣺###[###,###]��
		local a,b,c,localMap = role.GetRoleScriptData(TargetID, 4, RoleDataType["PositionA"], RoleDataType["PositionB"], RoleDataType["PositionC"], RoleDataType["PositionMap"])
	    local MsgID = msg.BeginMsgEvent()
	    msg.AddMsgEvent(MsgID, 71, 62)
	    msg.AddMsgEvent(MsgID, 6, localMap)
	    msg.AddMsgEvent(MsgID, 9, a)
	    msg.AddMsgEvent(MsgID, 9, c)
	    msg.DispatchRoleMsgEvent(TargetID, MsgID)
	    return 36, false
	else
		local bRet, bIgnore = 0, false
		--�жϱ������пռ��Ƿ��㹻
		local FreeSize = role.GetBagFreeSize(TargetID)
		if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
			bRet = 40
		end
		return bRet, bIgnore
	end
end

--ʹ���ƾ�ͼ������Ѱ�������ʻ�ã�
function I1350002_UseItem(MapID, InstanceID, TypeID, TargetID)
	--�ӵ����Ѱ�������Ϊ�˷�ֹ������񲻳ɹ�ȴ������ұ�ͼ�����Խ�������Ĳ����������
	role.AddQuest(TargetID, 20035)
	--�����ᱦͼ(1350005)
	role.QuestAddRoleItem(MapID, InstanceID, TargetID, 20035, 1350005, 1, -1, 300)
	--����һָ���Ŀ�ѡ�񳡾���ѡ��һ�������
	WaBao_GetPoint(MapID, InstanceID, TargetID)
	--�����ɵ�����㷢���ͻ���
	--��Ѱ���ص㣺###[###,###]��
	local localA,localB,localC,localMap = role.GetRoleScriptData(TargetID, 4, RoleDataType["PositionA"], RoleDataType["PositionB"], RoleDataType["PositionC"], RoleDataType["PositionMap"])
	--����ͼcrcֵ�س�����
	--local MapI = (localMap - localMap%100000)/100000
	--local MapII = localMap%100000

	local MsgID = msg.BeginMsgEvent()
	--msg.AddMsgEvent(MsgID, 13, 0)
	msg.AddMsgEvent(MsgID, 71, 62)
	msg.AddMsgEvent(MsgID, 6, localMap)
	--msg.AddMsgEvent(MsgID, 6, MapI)
	--msg.AddMsgEvent(MsgID, 6, MapII)
	msg.AddMsgEvent(MsgID, 9, localA)
	msg.AddMsgEvent(MsgID, 9, localC)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)
end

--ʹ����ᱦͼ(1350005)
function I1350005_UseItem(MapID, InstanceID, TypeID, TargetID)
	--����Ѿ�����ָ������㣬��ʼ�ڱ�
	XuanJi_DigTreasure(MapID, InstanceID, TargetID)
	--�������
	role.RoleCompleteQuest(TargetID, 20035, -1)
	--�ٴ������������
	WaBao_GetPoint(MapID, InstanceID, TargetID)
end

--�������
function x20035_OnComplete(mapID, instanceID, questID, ownerID, enderID)
end



--ȡ������
function x20035_OnDelete(mapID, instanceID, questID, ownerID)
	--ȡ������Ҫɾ����ᱦͼ
	role.RemoveFromRole(mapID, instanceID, ownerID, 1350005, 1, 103)
end

aux.RegisterItemEvent(1350002, 0, "I1350002_CanUseItem")
aux.RegisterItemEvent(1350005, 0, "I1350005_CanUseItem")
aux.RegisterItemEvent(1350002, 1, "I1350002_UseItem")
aux.RegisterItemEvent(1350005, 1, "I1350005_UseItem")
aux.RegisterQuestEvent(20035, 1, "x20035_OnComplete")
aux.RegisterQuestEvent(20035, 2, "x20035_OnDelete")

