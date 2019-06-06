--ˢ�ִ�����320
--���������1010506��
--X=278 Y=4202 Z=362
--X=300 Y=4216 Z=359
--X=275 Y=4215 Z=336
--X=297 Y=4200 Z=333
--ˢ�ִ�����418
--�����������������������4�ְ����ʱ��
--X=898 Y=7000 Z=146
--X=886 Y=7000 Z=170
--X=862 Y=7000 Z=146
--������������
--X=899 Y=7000 Z=158
--X=865 Y=7000 Z=166
--X=872 Y=7000 Z=137
--��������ˢ3�Σ�ÿ�μ��1.5���ӡ�ȫ����ɱ��ˢ������������1010527��
--ScriptData��0λΪ�ж��Ƿ񴥷���������320
--ScriptData��1λΪ�ж��Ƿ񴥷���������418
--ScriptData��2λΪ�ж��Ƿ񴥷���������743
--ScriptData��3λΪ�ж�����ˢ�ڼ���С��
--ScriptData��4λΪ�ж�BOSS�Ƿ�ɱ������������ֵΪ1���컨��ĸ��ֵΪ10.
--ScriptData��5λΪ�ж�ɱ������������������
--ScriptData��6λΪ�ж�ɱ��������������
--ScriptData��7-13λ���ڼ�ħʯ��
--ScriptData��14ΪС������
--ScriptData��15Ϊ��ȡ������������¼


function m45_OnEnterTrigger(MapID, InstanceID, RoleID, ObjID)
	if ObjID == 320 then
		local BOOL_EnterTrigger320 = map.GetMapScriptData(MapID,InstanceID,1,0) --����0��ֵΪ0��ˢ��
		if BOOL_EnterTrigger320 == 0 then --�ж��Ƿ񱻴�����
			map.SetMapScriptData(MapID,InstanceID,1,0,1)
		    map.MapCreateColCreature(MapID, InstanceID, 1010506, 278, 4202, 362, 1, "")
		    map.MapCreateColCreature(MapID, InstanceID, 1010506, 300, 4216, 359, 1, "")
		    map.MapCreateColCreature(MapID, InstanceID, 1010506, 275, 4215, 336, 1, "")
		    map.MapCreateColCreature(MapID, InstanceID, 1010506, 397, 4200, 333, 1, "")
		end
	elseif ObjID == 418 then
		local BOOL_EnterTrigger418 = map.GetMapScriptData(MapID,InstanceID,1,1) --����0��ֵΪ0��ˢ��
		if BOOL_EnterTrigger418 == 0 then --�ж��Ƿ񱻴�����
			map.SetMapScriptData(MapID,InstanceID,1,1,1)
			map.MapCreateColCreature(MapID, InstanceID, 1010503, 898, 7000, 146, 1, "")
			map.MapCreateColCreature(MapID, InstanceID, 1010503, 886, 7000, 170, 1, "")
		    map.MapCreateColCreature(MapID, InstanceID, 1010503, 862, 7000, 146, 1, "")
		    map.MapCreateColCreature(MapID, InstanceID, 1010505, 899, 7000, 158, 1, "")
		    map.MapCreateColCreature(MapID, InstanceID, 1010505, 865, 7000, 166, 1, "")
		    map.MapCreateColCreature(MapID, InstanceID, 1010505, 872, 7000, 137, 1, "")
			map.SetMapScriptData(MapID,InstanceID,1,3,1)--���øü���λΪ��һ��
			map.MapCreateColCreature(MapID, InstanceID, 3075402, 154, 4320, 657, 1, "")--͸����ʱ��1����ʱ��1�ְ�
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 1775)	--60���ˢ����һ������������
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)     --������Ϣ
		end
	elseif ObjID == 743 then
		local BOOL_EnterTrigger743 = map.GetMapScriptData(MapID,InstanceID,1,2)
		if BOOL_EnterTrigger743 == 0 then --�ж��Ƿ񱻴�����
			local i = role.GetRoleItemNum(RoleID, 2615801)
			local k = role.GetRoleItemNum(RoleID, 2615802)
			if i == 1 and k == 1 then
				map.SetMapScriptData(MapID,InstanceID,1,2,1)--��¼�Ѿ�������743
				map.OpenCloseDoor(MapID, InstanceID, 505, 1)
				role.RemoveFromRole(MapID, InstanceID, RoleID, 2615801, 1, 420)
				role.RemoveFromRole(MapID, InstanceID, RoleID, 2615802, 1, 420)
			else
			    local MsgID = msg.BeginMsgEvent()
                msg.AddMsgEvent(MsgID, 26, 524)	-- "�����ͬʱӵ������Կ�ײ���ͨ�����š�"
                msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			end
		end
	end
end
aux.RegisterMapEvent("m45",6,"m45_OnEnterTrigger")

--����
function m45_OnPlayerEnter(MapID, InstanceID,RoleID)
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 99996)
	if bhave == true then
	 unit.CancelBuff(MapID, InstanceID, RoleID, 9999601)
	 end
end

--ע��
aux.RegisterMapEvent("m45", 2, "m45_OnPlayerEnter")



