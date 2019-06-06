--[[BiWuQieCuo_Available = 0

--��������д賡������������BiWuQieCuo_Available��Ϊ2.
function BiWuQieCuo_OnInit(MapID,InstanceID,CreatureID,CreatureTypeID)
	BiWuQieCuo_Available = 1 + BiWuQieCuo_Available
end
aux.RegisterMapEvent("a04", 0, "BiWuQieCuo_OnInit")
aux.RegisterMapEvent("a05", 0, "BiWuQieCuo_OnInit")]]

--����m04ʱ����ȫ�ֱ���Open_Arena = 1����ˢ��NPC����������
function BiWuQieCuo_OnInit(MapID,InstanceID,CreatureID,CreatureTypeID)
	if Open_Arena == 1 then
		map.MapCreateColCreature(MapID, InstanceID, 4006512, 331, 2150, 307, 1, "")
	end
end
aux.RegisterMapEvent("a04", 0, "BiWuQieCuo_OnInit")
aux.RegisterMapEvent("a05", 0, "BiWuQieCuo_OnInit")

--��ɱ������ű�
function JinRuYanWuChang_KillPeople(MapID, InstanceID, RoleID, KillerID)
    local level1 = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local level2 = role.GetRoleLevel(MapID, InstanceID, KillerID)
	local a = level2 - level1
	if unit.IsPlayer(RoleID) and unit.IsPlayer(KillerID) then	  --�ж�˫��������
	    if a <= 20 then
	        local Killer_Num = role.GetRoleScriptData(KillerID, 1, RoleDataType["BiWuQieCuo_Kill"])
		    Killer_Num = Killer_Num + 1
		    role.SetRoleScriptData(KillerID, 1, RoleDataType["BiWuQieCuo_Kill"], Killer_Num)

		    if Killer_Num == 100 then
		        role.SigTitleEvent(KillerID, 186)    --������Ѫս��ƺ�
		        local MsgID = msg.BeginMsgEvent()
	            msg.AddMsgEvent(MsgID, 13, 1)
			    msg.AddMsgEvent(MsgID, 1, 2010)       --XXX��ɱ���������������䳡��ȡ��100��ʤ�������Ѫս��ƺţ�
	            msg.AddMsgEvent(MsgID, 2, KillerID)
	            msg.DispatchWorldMsgEvent(MsgID)
			elseif Killer_Num == 50 then
				local MsgID = msg.BeginMsgEvent()
	            msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 2011)       --XXX��ɱ���������������䳡�д����XXX����ɱ����������ȡ��50��ʤ��
				msg.AddMsgEvent(MsgID, 2, KillerID)
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.DispatchWorldMsgEvent(MsgID)
     		elseif Killer_Num >= 10 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 2012)       --XXX��ɱ���������������䳡��սʤ��XXX����ɱ����������ȡ��XX(��ʤ��)��ʤ��
				msg.AddMsgEvent(MsgID, 2, KillerID)
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.AddMsgEvent(MsgID, 9, Killer_Num)
				msg.DispatchMapMsgEvent(MsgID, MapID, -1)
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 0)
				msg.AddMsgEvent(MsgID, 1, 2013)       --XXX��ɱ���������������䳡��սʤ��XXX����ɱ����������
				msg.AddMsgEvent(MsgID, 2, KillerID)
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.DispatchMapMsgEvent(MsgID, MapID, -1)
			end

			local DeadPeople_Num = role.GetRoleScriptData(RoleID, 1, RoleDataType["BiWuQieCuo_Kill"])
			if DeadPeople_Num >= 100 then
				role.SigTitleEvent(KillerID, 187)    --����ս���ս��߳ƺ�
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 2014)       --XXX��ɱ�����������ս�����Ѫս�񡪡�XXX����ɱ����������XX(��ɱ����ʤ��)��ʤ�����ս���ս��߳ƺţ�
				msg.AddMsgEvent(MsgID, 2, KillerID)
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.AddMsgEvent(MsgID, 9, DeadPeople_Num)
				msg.DispatchWorldMsgEvent(MsgID)
			elseif DeadPeople_Num >= 50 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 2015)       --XXX��ɱ��������������һ��ս���XXX����ɱ����������XX(��ɱ����ʤ��)��ʤ����
				msg.AddMsgEvent(MsgID, 2, KillerID)
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.AddMsgEvent(MsgID, 9, DeadPeople_Num)
				msg.DispatchWorldMsgEvent(MsgID)
			elseif DeadPeople_Num >= 10 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 1)
				msg.AddMsgEvent(MsgID, 1, 2016)       --XXX��ɱ�����������ս���XXX����ɱ����������XX(��ɱ����ʤ��)��ʤ��
				msg.AddMsgEvent(MsgID, 2, KillerID)
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.AddMsgEvent(MsgID, 9, DeadPeople_Num)
				msg.DispatchMapMsgEvent(MsgID, MapID, -1)
			end
		else
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 0)
			msg.AddMsgEvent(MsgID, 1, 2017)       -- "��ɱ�ȼ����Լ����̫�����Ҳ���������ɱ����"
			msg.DispatchRoleMsgEvent(KillerID, MsgID)     --������Ϣ
		end
	end
    role.SetRoleScriptData(RoleID, 1, RoleDataType["BiWuQieCuo_Kill"], 0)
end


aux.RegisterMapEvent("a04", 5, "JinRuYanWuChang_KillPeople")
aux.RegisterMapEvent("a05", 5, "JinRuYanWuChang_KillPeople")



--�������䳡�㲥
function JinRuYanWuChang(MapID, InstanceID, RoleID)
	local kill = role.GetRoleScriptData(RoleID, 1, RoleDataType["BiWuQieCuo_Kill"])
	if kill >= 10 then
	    local MsgID = msg.BeginMsgEvent()
	    msg.AddMsgEvent(MsgID, 13, 0)
	    msg.AddMsgEvent(MsgID, 1, 2018)       --XXX���������䳡�����Ѿ������䳡�л����XX��ʤ����
	    msg.AddMsgEvent(MsgID, 2, RoleID)
	    msg.AddMsgEvent(MsgID, 9, kill)
	    msg.DispatchMapMsgEvent(MsgID, MapID, -1)
	else
	    local MsgID = msg.BeginMsgEvent()
	    msg.AddMsgEvent(MsgID, 13, 0)
	    msg.AddMsgEvent(MsgID, 1, 2019)       --XXX���������䳡��
	    msg.AddMsgEvent(MsgID, 2, RoleID)
	    msg.DispatchMapMsgEvent(MsgID, MapID, -1)
	end
end


aux.RegisterMapEvent("a04", 2, "JinRuYanWuChang")
aux.RegisterMapEvent("a05", 2, "JinRuYanWuChang")


--�����䳡����ʯ3021023�Ի������͵���ȫ��
function n3021023_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 2020) --    ����ص����䳡��ս̨��
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		msg.AddMsgEvent(MsgID, 21, 4) -- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 21) -- "ȷ��"
		msg.AddMsgEvent(MsgID, 21, 6)   --ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 22)   --��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
    elseif TalkIndex == 4 then
        role.RoleGotoNewMap(MapID, InstanceID, RoleID, MapID, 332, 43, 319)--���͵���ȫ��
    end
end

aux.RegisterCreatureEvent(3021023, 7, "n3021023_OnTalk")


--��Ҹ���ű�
function BiWuQieCuo_RoloRevive(MapID, InstanceID, RoleID, Type, ReviveHP, ReviveMP, x, y, z, RebornMapID)

    ReviveHP = role.GetRoleAttValue(MapID, InstanceID, RoleID, 8)
	ReviveMP = role.GetRoleAttValue(MapID, InstanceID, RoleID, 10)
	return ReviveHP, ReviveMP, 332, 2155, 319, MapID
end

aux.RegisterMapEvent("a04", 11, "BiWuQieCuo_RoloRevive")
aux.RegisterMapEvent("a05", 11, "BiWuQieCuo_RoloRevive")
