function m03_CanInviteJoinTeam(MapID, InstanceID, RoleID)         --ע�⣡ԭ���Ľű��¼��в�û��RoleID���أ���Ҫ�����¼ӡ�RoleIDΪ����������������
    local bool = true
	if act.GetActIsStart(161) or act.GetActIsStart(162) then
		if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 11409) then--��������ħ���Ŷ�״̬��5λID�������ܽ��иò���
			bool = false
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26,	12908)		--�����Ŷ���Ҳ���ִ�иò���
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
	return bool
end

function m03_CanLeaveTeam(MapID, InstanceID, RoleID)               --ע�⣡ԭ���Ľű��¼��в�û��RoleID���أ���Ҫ�����¼ӡ�RoleIDΪ����������������
	local bool = true
	if act.GetActIsStart(161) or act.GetActIsStart(162) then
		if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 11409) then--��������ħ���Ŷ�״̬��5λID�������ܽ��иò���
			bool = false
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26,	12908)		--�����Ŷ���Ҳ���ִ�иò���
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
	return bool
end

function m03_CanChangeLeader(MapID, InstanceID, RoleID)         --ע�⣡ԭ���Ľű��¼��в�û��RoleID���أ���Ҫ�����¼ӡ�RoleIDΪ����������������
	local bool = true
	if act.GetActIsStart(161) or act.GetActIsStart(162) then
		if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 11409) then--��������ħ���Ŷ�״̬��5λID�������ܽ��иò���
			bool = false
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26,	12908)		--�����Ŷ���Ҳ���ִ�иò���
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
	return bool
end

function m03_CanKickMember(MapID, InstanceID, RoleID)          --ע�⣡ԭ���Ľű��¼��в�û��RoleID���أ���Ҫ�����¼ӡ�RoleIDΪ����������������
	local bool = true
	if act.GetActIsStart(161) or act.GetActIsStart(162) then
		if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 11409) then--��������ħ���Ŷ�״̬��5λID�������ܽ��иò���
			bool = false
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26,	12908)		--�����Ŷ���Ҳ���ִ�иò���
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
	return bool
end

aux.RegisterMapEvent("m03",  8, "m03_CanInviteJoinTeam")
aux.RegisterMapEvent("m03",  9, "m03_CanLeaveTeam")
aux.RegisterMapEvent("m03",  10, "m03_CanChangeLeader")
aux.RegisterMapEvent("m03",  17, "m03_CanKickMember")

function m03_LeaveMap(MapID, InstanceID, RoleID)
    if act.GetActIsStart(161) or act.GetActIsStart(162) then
		if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 11409) then--��������ħ���Ŷ�״̬��5λID�������ܽ��иò���
			local bool = 0
			local index = 0
			for k,v in pairs(Jidou.Singlewaitinglist) do
				if v == RoleID then
					bool = 1
					index = k
				end
			end
			for k,v in pairs(Jidou.Teamwaitinglist) do
				if v == RoleID then
					bool = 2
					index = k
				end
			end
			if bool == 1 then
				unit.CancelBuff(MapID, InstanceID,RoleID, 1140901) --ȡ��ħ���Ŷ�״̬
				table.remove(Jidou.Singlewaitinglist,index)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 71, 12909)			--�����뿪��ħ�񶷼������Ŷ�����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			elseif bool == 2 then
				local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
				if TeamID ~= 4294967295 then
					if index < 5 then
						for i = 1,4 do
							local i = 5-i          --Ϊ�˴Ӻ���ǰɾ�����
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 71, 12910)			--�����뿪���裬�����뿪��ħ�񶷼������Ŷ�����
							msg.DispatchRoleMsgEvent(Jidou.Teamwaitinglist[i], MsgID)
							unit.CancelBuff(MapID, InstanceID,Jidou.Teamwaitinglist[i], 1140901)--ȡ��ħ���Ŷ�״̬
							table.remove(Jidou.Teamwaitinglist,i)
						end
					elseif index < 9 then
						for i = 5,8 do
							local i = 13-i			 --Ϊ�˴Ӻ���ǰɾ�����
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 71, 12910)			--�����뿪���裬�����뿪��ħ�񶷼������Ŷ�����
							msg.DispatchRoleMsgEvent(Jidou.Teamwaitinglist[i], MsgID)
							unit.CancelBuff(MapID, InstanceID,Jidou.Teamwaitinglist[i], 1140901)--ȡ��ħ���Ŷ�״̬
							table.remove(Jidou.Teamwaitinglist,i)
						end
					end
				end
			end
		end
	end
end

aux.RegisterMapEvent("m03", 3, "m03_LeaveMap")
