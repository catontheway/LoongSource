--�������ͷ3021017�Ի������ͽ����䳡��
function n3021017_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    if TalkIndex == -1 then
	    --if BiWuQieCuo_Available == 2 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 2001) -- "������ͷ��\n    ���������ľ�����Щֻ��ֽ��̸���ļһ��Ҫ��õ�������ʶ���ͱ��������䳡��֤���Լ���������\n    С��ʾ�����䳡�ǹ����PK��֮�ã��������䳡�������Լ��Ķ���֮�⣬��ҿ��Թ������е�������Ҷ��������κγͷ���"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			--msg.AddMsgEvent(MsgID, 21, 4) -- ȷ����ť
			--msg.AddMsgEvent(MsgID, 1, 2002) -- "����80���������䳡"
			msg.AddMsgEvent(MsgID, 21, 5) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 2003) -- "����81���������䳡"
			msg.AddMsgEvent(MsgID, 21, 6)   --ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 2004)   --���´���˵�ɡ�
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		--[[else
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 2021) -- "������ͷ��\n    ���������ľ�����Щֻ��ֽ��̸���ļһ��Ҫ��õ�������ʶ���ͱ��������䳡��֤���Լ���������\n    С��ʾ�����䳡�ǹ����PK��֮�ã��������䳡�������Լ��Ķ���֮�⣬��ҿ��Թ������е�������Ҷ��������κγͷ���\n    (���䳡�������ţ������ڴ���)"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		end]]
    --[[elseif TalkIndex == 4 then
        local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		if level ~=nil then
		    if level >= 30 then
			    if level <= 80 then
					role.RoleGotoNewMap(MapID, InstanceID, RoleID, 2092184628, 332, 43, 319)--����a04
				else
				    local MsgID = msg.BeginMsgEvent()
		            msg.AddMsgEvent(MsgID, 20, 2005) -- "���ĵȼ�̫�ߣ����ܽ���ͼ����䳡��"
 		            msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
                    msg.AddMsgEvent(MsgID, 21, 6) -- ȷ����ť
                    msg.AddMsgEvent(MsgID, 1, 21) -- "ȷ��"
		            msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
				end
			else
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 2006) -- "���ĵȼ�̫�ͣ����ܽ������䳡��"
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, 6) -- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 21) -- "ȷ��"
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			end
        end]]
    elseif TalkIndex == 5 then
        --local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		--if level ~=nil then
		    --if level >= 30 then
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 2092184628, 332, 43, 319)--����a04
			--[[else
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 2006) -- "���ĵȼ�̫�ͣ����ܽ������䳡��"
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, 6) -- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 21) -- "ȷ��"
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			end
        end]]
    end
end

aux.RegisterCreatureEvent(3021017, 7, "n3021017_OnTalk")


--�������ͷ3021018�Ի������͵���¹�Ƶ۳ǡ�
function n3021018_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 2007) -- "������ͷ��\n    ���뿪�����ʱ�����˵һ���������Ի������ȥ��"
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		msg.AddMsgEvent(MsgID, 21, 4) -- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 2008) -- "�뿪���䳡���ص��Ƶ۳�"
		msg.AddMsgEvent(MsgID, 21, 6)   --ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 2009)   --������ʱ�����뿪��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
    elseif TalkIndex == 4 then
        role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3017299663, 2102, 367, 2650)--�ص��Ƶ۳�
    end
end

aux.RegisterCreatureEvent(3021018, 7, "n3021018_OnTalk")
