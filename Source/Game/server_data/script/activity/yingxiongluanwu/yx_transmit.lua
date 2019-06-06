--NPC���ͽ��볡��todo

--�ж�����Ƿ�Ϊ�������뿪�
local function IsDeadLeave(MapID, InstanceID, RoleID, RoleLevel)
    if Act14_RoleData[RoleID] then
		if RoleLevel < 60 then
			return true, 2000
		elseif RoleLevel < 80 then
			return true, 5000
		elseif RoleLevel < 100 then
			return true, 10000
		else
			return true, 20000
		end
	else
	    return false, 0
	end
end

function C4500101_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	--�жϻ�Ƿ��Ѿ���ʼ
	local bStart = false
	if act.GetActIsStart(14) or act.GetActIsStart(97) or act.GetActIsStart(18) or act.GetActIsStart(103) then
	    bStart = true
	end

	if -1 == TalkIndex then
	    if PVPactivity_trigger == 1 then
			--��Ѿ���ʼ
			if bStart then
				local RoleLevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
				if RoleLevel < 40 then
					--��ʾ��ҵȼ��������޷�����
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20,	157)
					msg.AddMsgEvent(MsgID, 21,	5)			    --ȡ��
					msg.AddMsgEvent(MsgID, 1,	22)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
					return
				end

				--���������������뿪����ʾ����ٴν�����Ҫ���ѽ�Ǯ
				local bDeadLeave, Money = IsDeadLeave(MapID, InstanceID, RoleID, RoleLevel)
				if bDeadLeave then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 151)
					msg.AddMsgEvent(MsgID, 9, Money)
					msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
					msg.AddMsgEvent(MsgID, 21,	4)			--ȷ��
					msg.AddMsgEvent(MsgID, 1,	21)
					msg.AddMsgEvent(MsgID, 21,	5)			--ȡ��
					msg.AddMsgEvent(MsgID, 1,	22)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					--��ʾ����Ƿ��������
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20,	150)		--���Ƿ�Ҫ����Ӣ��������
					msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
					msg.AddMsgEvent(MsgID, 21,	4)			--ȷ��
					msg.AddMsgEvent(MsgID, 1,	21)
					msg.AddMsgEvent(MsgID, 21,	5)			--ȡ��
					msg.AddMsgEvent(MsgID, 1,	22)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else --���û�п�ʼ
				--��ʾ��һû�п�ʼ
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 152)			--Ӣ��������û�п�ʼ��
				msg.AddMsgEvent(MsgID, 21,	5)			    --ȡ��
				msg.AddMsgEvent(MsgID, 1,	22)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		else
		    --��ʾ��һû�п�ʼ
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 2862)			--Ӣ�������ݲ����ţ������½⡣
			msg.AddMsgEvent(MsgID, 21,	5)			    --ȡ��
			msg.AddMsgEvent(MsgID, 1,	22)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end

	--������ѡ��ȷ��
	if TalkIndex == 4 then
		--������ҵ������
		local Index = math.random(1, 8)
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 2983744463, m21_BornPos[Index].x, m21_BornPos[Index].y, m21_BornPos[Index].z)
	end
end

--ע����ҽ����¼�
aux.RegisterCreatureEvent(4500101, 7, "C4500101_OnTalk")
