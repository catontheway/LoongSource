--���ܡ�������
--����ID����9000101
--���ܡ����ջ�
--����ID����9000001

--���ɼ����Ƿ����ʹ��
function s9000001_CanCast(mapid, instanceid, skillid, ownerid, resid)
	local bRet = role.CanGather(ownerid, resid) 
	return bRet
end

--�ջ����Ƿ����ʹ��
function s9000101_CanCast(mapid, instanceid, skillid, ownerid, resid)
	local bRet = role.CanGather(ownerid, resid) 
	return bRet
end

--ע��
aux.RegisterSkillEvent(9000001, 0, "s9000001_CanCast")
aux.RegisterSkillEvent(9000002, 0, "s9000001_CanCast")
aux.RegisterSkillEvent(9000003, 0, "s9000001_CanCast")
aux.RegisterSkillEvent(9000004, 0, "s9000001_CanCast")
aux.RegisterSkillEvent(9000005, 0, "s9000001_CanCast")
aux.RegisterSkillEvent(9000006, 0, "s9000001_CanCast")
aux.RegisterSkillEvent(9000007, 0, "s9000001_CanCast")
aux.RegisterSkillEvent(9000008, 0, "s9000001_CanCast")
aux.RegisterSkillEvent(9000009, 0, "s9000001_CanCast")
aux.RegisterSkillEvent(9000010, 0, "s9000001_CanCast")

aux.RegisterSkillEvent(9000101, 0, "s9000101_CanCast")
aux.RegisterSkillEvent(9000102, 0, "s9000101_CanCast")
aux.RegisterSkillEvent(9000103, 0, "s9000101_CanCast")
aux.RegisterSkillEvent(9000104, 0, "s9000101_CanCast")
aux.RegisterSkillEvent(9000105, 0, "s9000101_CanCast")
aux.RegisterSkillEvent(9000106, 0, "s9000101_CanCast")
aux.RegisterSkillEvent(9000107, 0, "s9000101_CanCast")
aux.RegisterSkillEvent(9000108, 0, "s9000101_CanCast")
aux.RegisterSkillEvent(9000109, 0, "s9000101_CanCast")
aux.RegisterSkillEvent(9000110, 0, "s9000101_CanCast")
