--�Ƿ����ʹ����ɱͼ,�ƾ�ͼ��̰��ͼ
function I1350001_CanUseItem(MapID, InstanceID, TypeID, TargetID)
    if role.IsRoleHaveQuest(MapID, InstanceID, TargetID, 20035) or role.IsRoleHaveQuest(MapID, InstanceID, TargetID, 20036) then
    	return 45, false
    elseif role.IsRoleHaveQuest(MapID, InstanceID, TargetID, 20034) then
    	return 41, false
    end

	--�жϱ����Ƿ��еط������Ǳ�ͼ
	local BagFreeSize = role.GetBagFreeSize(TargetID)
	if(BagFreeSize <= 0) then
		return 40, false
	end

	return 0, false
end

--�Ƿ����ʹ�����Ǳ�ͼ
function I1350004_CanUseItem(MapID, InstanceID, TypeID, TargetID)
	--�������ѵ�����ָ�������
	local bool = WaBao_EnterNearArea(MapID, InstanceID, TargetID)
	--�����Ҳ���ָ�����긽������ָ�����귢���ͻ���
	if bool == false then
		--��ʾ��� ��Ѱ���ص㣺###[###,###]��
		local a,b,c,localMap = role.GetRoleScriptData(TargetID, 4, RoleDataType["PositionA"], RoleDataType["PositionB"], RoleDataType["PositionC"], RoleDataType["PositionMap"])
	  --����ͼcrcֵ�س�����
		--local MapI = (localMap - localMap%100000)/100000
		--local MapII = localMap%100000

	    local MsgID = msg.BeginMsgEvent()
	   -- msg.AddMsgEvent(MsgID, 13, 0)
	    msg.AddMsgEvent(MsgID, 71, 62)
	    msg.AddMsgEvent(MsgID, 6, localMap)
	    --msg.AddMsgEvent(MsgID, 6, MapI)
			--msg.AddMsgEvent(MsgID, 6, MapII)
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

--ʹ����ɱͼ���������������ã�
function I1350001_UseItem(MapID, InstanceID, TypeID, TargetID)
	--�ӵ�����Ѱ�������
  role.AddQuest(TargetID, 20034)
	--������Ǳ�ͼ(1350004)
	role.QuestAddRoleItem(MapID, InstanceID, TargetID, 20034, 1350004, 1, -1, 300)
	--�������ĳ�������е�һ�������
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

--ʹ�����Ǳ�ͼ
function I1350004_UseItem(MapID, InstanceID, TypeID, TargetID)
	--����ָ�������,��ʼ�ڱ�
	QiXing_DigTreasure(MapID, InstanceID, TargetID)
	--�������
	role.RoleCompleteQuest(TargetID, 20034, -1)
	--�ٴ������������
	WaBao_GetPoint(MapID, InstanceID, TargetID)
end

--�������
function x20034_OnComplete(mapID, instanceID, questID, ownerID, enderID)
end

--ȡ������
function x20034_OnDelete(mapID, instanceID, questID, ownerID)
	--ȡ������Ҫɾ�����Ǳ�ͼ
	role.RemoveFromRole(mapID, instanceID, ownerID, 1350004, 1, 103)
end


aux.RegisterItemEvent(1350001, 0, "I1350001_CanUseItem")
aux.RegisterItemEvent(1350004, 0, "I1350004_CanUseItem")
aux.RegisterItemEvent(1350001, 1, "I1350001_UseItem")
aux.RegisterItemEvent(1350004, 1, "I1350004_UseItem")
aux.RegisterQuestEvent(20034, 1, "x20034_OnComplete")
aux.RegisterQuestEvent(20034, 2, "x20034_OnDelete")

